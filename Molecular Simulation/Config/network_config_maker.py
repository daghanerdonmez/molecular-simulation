import xml.etree.ElementTree as ET
import yaml
import re

def parse_graphml(file_path):
    tree = ET.parse(file_path)
    root = tree.getroot()
    
    # Extract namespaces dynamically
    ns_map = {n[0]: n[1] for n in [x.split('}') for x in root.tag[1:].split('{') if '}' in x]}
    graphml_ns = ns_map.get("", "http://graphml.graphdrawing.org/xmlns")
    y_ns = ns_map.get("y", "http://www.yworks.com/xml/graphml")
    
    namespace = {"graphml": graphml_ns, "y": y_ns}
    print("Extracted Namespace:", namespace)  # Debugging
    
    nodes = {}
    edges = []
    
    # Parse nodes
    for node in root.findall(".//graphml:node", namespace):
        node_id = node.get("id")
        label_elem = node.find(".//y:NodeLabel", namespace)
        
        print(f"Processing Node: {node_id}")  # Debugging
        
        length = 0.0
        radius = 0.0
        
        if label_elem is None or not label_elem.text:
            print(f"Node {node_id} has no label or text.")  # Debugging
        else:
            print(f"Node {node_id} Label Text: {label_elem.text}")  # Debugging
            match = re.search(r'length=([0-9.eE-]+),\s*radius=([0-9.eE-]+)', label_elem.text)
            if match:
                length = float(match.group(1))
                radius = float(match.group(2))
                print(f"Extracted: length={length}, radius={radius}")  # Debugging
            else:
                print(f"Failed to extract parameters from node {node_id} label text.")
        
        nodes[node_id] = {
            "particle_count": 0,
            "length": length,
            "radius": radius,
            "left_connections": [],
            "right_connections": []
        }
    
    print("Parsed Nodes:", nodes)  # Debugging
    
    # Parse edges
    for edge in root.findall(".//graphml:edge", namespace):
        source = edge.get("source")
        target = edge.get("target")
        edges.append((source, target))
        print(f"Edge found: {source} -> {target}")  # Debugging
    
    # Determine connections
    for source, target in edges:
        if source in nodes and target in nodes:
            nodes[source]["right_connections"].append(target)
            nodes[target]["left_connections"].append(source)
            print(f"Updated Connections: {source} -> {target}")  # Debugging
    
    print("Final Node Data:", nodes)  # Debugging
    return {"pipes": nodes}

def save_as_yaml(data, output_file):
    with open(output_file, "w") as f:
        yaml.dump(data, f, default_flow_style=False)

# Example usage
input_file = "/Users/daghanerdonmez/Desktop/Molecular-Simulation/Molecular Simulation/Config/graph.graphml"  # Replace with actual input file
output_file = "/Users/daghanerdonmez/Desktop/Molecular-Simulation/Molecular Simulation/Config/network_config.yaml"

data = parse_graphml(input_file)
save_as_yaml(data, output_file)

print("YAML file generated successfully.")