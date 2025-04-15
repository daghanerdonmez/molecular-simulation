import json
import sys

def convert_node_red_to_config(input_file, output_file):
    # Load the JSON data
    with open(input_file, 'r') as f:
        data = json.load(f)
    
    # Extract nodes of type "simulation-config"
    config_nodes = [node for node in data if node.get('type') == 'simulation-config']
    
    # Extract nodes of type "flow" to get flowValue
    flow_nodes = [node for node in data if node.get('type') == 'flow']
    flow_value = None
    if flow_nodes:
        flow_value = flow_nodes[0].get('flowValue', '0')
    
    # Initialize the config file content
    config_content = """//
//  config.h

#ifndef config_h
#define config_h

"""
    
    # Add simulation configuration parameters if available
    if config_nodes:
        config_node = config_nodes[0]  # Take the first config node
        
        # Add configuration parameters
        config_content += f"#define OUTPUT_RESULTS {'true' if config_node.get('outputResults', False) else 'false'}\n"
        config_content += f"#define GRAPHICS_ON {'true' if config_node.get('graphicsOn', True) else 'false'}\n\n"
        
        # Mode (0 for single simulation, 1 for simulation network)
        config_content += f"// 0 for single simulation\n"
        config_content += f"// 1 for simulation network\n"
        config_content += f"// If simulation network is chosen, applied boundary is automatically cylinder boundary\n"
        config_content += f"#define MODE {config_node.get('mode', 1)}\n\n"
        
        # Time and iteration parameters
        config_content += f"#define TIME_TO_RUN {config_node.get('timeToRun', 500)}\n"
        
        # Handle dt as string or number
        dt_value = config_node.get('dt', 0.01)
        if isinstance(dt_value, str):
            dt_value = float(dt_value)
        config_content += f"#define DT {dt_value}\n"
        
        config_content += f"#define NUMBER_OF_ITERATIONS (int)(TIME_TO_RUN/DT)\n"
        config_content += f"#define ITERATIONS_PER_FRAME {config_node.get('iterationsPerFrame', 10)}\n\n"
        
        # Diffusion coefficient
        diffusion_value = config_node.get('diffusionCoefficient', 7.94e-11)
        if isinstance(diffusion_value, str):
            diffusion_value = float(diffusion_value)
        config_content += f"#define D {diffusion_value:.2e} // diffusion coefficient\n\n"
        
        # Graphics zoom multiplier
        zoom_value = config_node.get('graphicsZoomMultiplier', 1000)
        if isinstance(zoom_value, str):
            zoom_value = float(zoom_value)
        config_content += f"#define GRAPHICS_ZOOM_MULTIPLIER {zoom_value:.0e}\n"
    
    # Add flow value if available
    if flow_value is not None:
        config_content += f"\n#define FLOW_VALUE {flow_value}\n"
    
    # Close the header guard
    config_content += "\n#endif /* config_h */\n"
    
    # Write config to file
    with open(output_file, 'w') as f:
        f.write(config_content)
    
    print(f"Conversion complete. Config file saved to {output_file}")
    if flow_value is not None:
        print(f"Added FLOW_VALUE = {flow_value} from flow node")

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python node-red-to-config.py input.json output.h")
        sys.exit(1)
    
    input_file = sys.argv[1]
    output_file = sys.argv[2]
    convert_node_red_to_config(input_file, output_file)
