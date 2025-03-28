import json
import yaml
import sys
from collections import defaultdict

def convert_node_red_to_yaml(input_file, output_file):
    # Load the JSON data
    with open(input_file, 'r') as f:
        data = json.load(f)
    
    # Extract nodes of type "pipe", "flow", and "sink"
    pipes = [node for node in data if node.get('type') == 'pipe']
    flows = [node for node in data if node.get('type') == 'flow']
    sinks = [node for node in data if node.get('type') == 'sink']
    
    # Create a mapping of node IDs to node names
    node_names = {}
    for idx, pipe in enumerate(pipes):
        # Check if pipe has a name and it's not empty
        if pipe.get('name') and pipe['name'].strip():
            node_names[pipe['id']] = f"pipe{idx+1}-{pipe['name'].strip()}"
        else:
            node_names[pipe['id']] = f"pipe{idx+1}"
    
    # Add sink nodes to the mapping
    for idx, sink in enumerate(sinks):
        if sink.get('name') and sink['name'].strip():
            node_names[sink['id']] = f"sink{idx+1}-{sink['name'].strip()}"
        else:
            node_names[sink['id']] = f"sink{idx+1}"
    
    # Initialize connection tracking
    left_connections = defaultdict(list)
    
    # Build the YAML structure
    yaml_data = {'pipes': {}, 'sinks': {}}
    
    # Track connections
    for pipe in pipes:
        pipe_id = pipe['id']
        wires = pipe.get('wires', [[]])[0]
        
        for target_id in wires:
            if target_id in node_names:  # Consider both pipe and sink type nodes
                # This is a right connection for the current pipe
                # Also record it as a left connection for the target
                left_connections[target_id].append(pipe_id)
    
    # Create a mapping of all nodes by ID for easy lookup
    all_nodes = {node['id']: node for node in data}
    
    # Track flow values for each pipe
    flow_values = {}
    
    # Process flow nodes first
    for flow_node in flows:
        flow_value = float(flow_node.get('flowValue', 0))
        wires = flow_node.get('wires', [[]])[0]
        
        # Assign flow value to the first pipe connected to this flow node
        for target_id in wires:
            if target_id in node_names:  # Only consider pipe type nodes
                flow_values[target_id] = flow_value
    
    # Propagate flow values through the network
    def calculate_flow_distribution(pipe_id, visited=None):
        if visited is None:
            visited = set()
        
        if pipe_id in visited:
            return  # Avoid cycles
        
        visited.add(pipe_id)
        
        # If this pipe already has a flow value, propagate it to connected pipes
        if pipe_id in flow_values:
            pipe_node = all_nodes[pipe_id]
            wires = pipe_node.get('wires', [[]])[0]
            
            # Get the right connections that are pipe nodes
            right_connections = [target_id for target_id in wires if target_id in node_names]
            
            if right_connections:
                # Calculate the sum of the fourth power of radiuses
                radius_power_sum = sum(
                    float(all_nodes[target_id].get('radius', 0.01)) ** 4 
                    for target_id in right_connections
                    if all_nodes[target_id].get('type') == 'pipe'  # Only consider pipes for flow calculation
                )
                
                if radius_power_sum > 0:
                    # Distribute flow proportionally to the fourth power of radius
                    for target_id in right_connections:
                        if all_nodes[target_id].get('type') == 'pipe':  # Only propagate flow to pipes
                            target_radius = float(all_nodes[target_id].get('radius', 0.01))
                            flow_fraction = (target_radius ** 4) / radius_power_sum
                            flow_values[target_id] = flow_values[pipe_id] * flow_fraction
                            
                            # Recursively calculate flow for downstream pipes
                            calculate_flow_distribution(target_id, visited)
    
    # Start flow calculation from pipes directly connected to flow nodes
    for flow_node in flows:
        for target_id in flow_node.get('wires', [[]])[0]:
            if target_id in node_names:
                calculate_flow_distribution(target_id)
    
    # Populate the YAML data for pipes
    for pipe in pipes:
        pipe_id = pipe['id']
        name = node_names[pipe_id]
        
        # Get connections
        right_connections = []
        for target_id in pipe.get('wires', [[]])[0]:
            if target_id in node_names:
                # Check if it's a sink or a pipe
                if all_nodes[target_id].get('type') == 'sink':
                    right_connections.append(node_names[target_id])
                else:
                    right_connections.append(node_names[target_id])
        
        left_conns = [node_names[source_id] for source_id in left_connections[pipe_id]]
        
        # Create pipe entry
        pipe_data = {
            'length': float(pipe.get('length', 0.1)),
            'radius': float(pipe.get('radius', 0.01)),
            'particle_count': 0,
            'left_connections': left_conns,
            'right_connections': right_connections
        }
        
        # Add flow value if available
        if pipe_id in flow_values:
            pipe_data['flow'] = flow_values[pipe_id]
        
        # Add receivers if present
        if pipe.get('receivers'):
            pipe_data['receivers'] = []
            # Track the order of receivers for naming
            receiver_count = 1
            for receiver in pipe.get('receivers', []):
                # Convert string values to appropriate types where possible
                receiver_data = {}
                for key, value in receiver.items():
                    try:
                        receiver_data[key] = float(value) if key != 'type' else value
                    except (ValueError, TypeError):
                        receiver_data[key] = value
                
                # Add a name property based on the order and type of the receiver
                if 'type' in receiver_data:
                    receiver_data['name'] = f"#{receiver_count}-{receiver_data['type']}"
                    receiver_count += 1
                
                pipe_data['receivers'].append(receiver_data)
        
        # Add emitters if present
        if pipe.get('emitters'):
            pipe_data['emitters'] = []
            for emitter in pipe.get('emitters', []):
                # Convert string values to appropriate types where possible
                emitter_data = {}
                for key, value in emitter.items():
                    try:
                        # Only convert numeric values to float, keep strings as is
                        if key not in ['emitter_pattern', 'emitter_pattern_type']:
                            emitter_data[key] = float(value)
                        else:
                            emitter_data[key] = value
                    except (ValueError, TypeError):
                        emitter_data[key] = value
                
                # Set default pattern type if not specified
                if 'emitter_pattern_type' not in emitter_data:
                    emitter_data['emitter_pattern_type'] = 'repeat'
                    
                pipe_data['emitters'].append(emitter_data)
        
        # Add to YAML structure
        yaml_data['pipes'][name] = pipe_data
    
    # Populate the YAML data for sinks
    for sink in sinks:
        sink_id = sink['id']
        name = node_names[sink_id]
        
        # Get left connections (pipes that connect to this sink)
        left_conns = [node_names[source_id] for source_id in left_connections[sink_id]]
        
        # Create sink entry
        sink_data = {
            'left_connections': left_conns
        }
        
        # Add to YAML structure
        yaml_data['sinks'][name] = sink_data
    
    # Write YAML to file
    with open(output_file, 'w') as f:
        yaml.dump(yaml_data, f, default_flow_style=False, sort_keys=False)
    
    print(f"Conversion complete. YAML file saved to {output_file}")

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python node-red-to-yaml.py input.json output.yaml")
        sys.exit(1)
    
    input_file = sys.argv[1]
    output_file = sys.argv[2]
    convert_node_red_to_yaml(input_file, output_file)