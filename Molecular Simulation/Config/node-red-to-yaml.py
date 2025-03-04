import json
import yaml
import sys
from collections import defaultdict

def convert_node_red_to_yaml(input_file, output_file):
    # Load the JSON data
    with open(input_file, 'r') as f:
        data = json.load(f)
    
    # Extract nodes of type "pipe"
    pipes = [node for node in data if node.get('type') == 'pipe']
    
    # Create a mapping of node IDs to node names
    node_names = {}
    for idx, pipe in enumerate(pipes):
        node_names[pipe['id']] = f"pipe{idx}"
    
    # Initialize connection tracking
    left_connections = defaultdict(list)
    
    # Build the YAML structure
    yaml_data = {'pipes': {}}
    
    # Track connections
    for pipe in pipes:
        pipe_id = pipe['id']
        wires = pipe.get('wires', [[]])[0]
        
        for target_id in wires:
            if target_id in node_names:  # Only consider pipe type nodes
                # This is a right connection for the current pipe
                # Also record it as a left connection for the target
                left_connections[target_id].append(pipe_id)
    
    # Populate the YAML data
    for pipe in pipes:
        pipe_id = pipe['id']
        name = node_names[pipe_id]
        
        # Get connections
        right_connections = [node_names[target_id] for target_id in pipe.get('wires', [[]])[0] 
                           if target_id in node_names]
        left_conns = [node_names[source_id] for source_id in left_connections[pipe_id]]
        
        # Create pipe entry
        pipe_data = {
            'length': float(pipe.get('length', 0.1)),
            'radius': float(pipe.get('radius', 0.01)),
            'particle_count': 0,
            'left_connections': left_conns,
            'right_connections': right_connections
        }
        
        # Add receivers if present
        if pipe.get('receivers'):
            pipe_data['receivers'] = []
            for receiver in pipe.get('receivers', []):
                # Convert string values to appropriate types where possible
                receiver_data = {}
                for key, value in receiver.items():
                    try:
                        receiver_data[key] = float(value) if key != 'type' else value
                    except (ValueError, TypeError):
                        receiver_data[key] = value
                pipe_data['receivers'].append(receiver_data)
        
        # Add emitters if present
        if pipe.get('emitters'):
            pipe_data['emitters'] = []
            for emitter in pipe.get('emitters', []):
                # Convert string values to appropriate types where possible
                emitter_data = {}
                for key, value in emitter.items():
                    try:
                        emitter_data[key] = float(value)
                    except (ValueError, TypeError):
                        emitter_data[key] = value
                pipe_data['emitters'].append(emitter_data)
        
        # Add to YAML structure
        yaml_data['pipes'][name] = pipe_data
    
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