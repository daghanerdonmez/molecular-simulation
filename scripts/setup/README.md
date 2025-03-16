# Setup Scripts

This directory contains scripts to help set up the Molecular Simulation project environment.

## Available Scripts

### `setup_node_red.sh` (Linux/macOS)

This script sets up Node-RED and installs the custom nodes required for the Molecular Simulation project.

#### What it does:

1. Checks if Node.js and npm are installed
2. Installs Node-RED globally if not already installed
3. Clones the custom Node-RED nodes repository
4. Installs the custom pipe nodes to your Node-RED installation

#### Usage:

```bash
# Make sure the script is executable
chmod +x setup_node_red.sh

# Run the script
./setup_node_red.sh
```

### `setup_node_red.ps1` (Windows)

This is the Windows PowerShell version of the setup script.

#### Usage:

```powershell
# You might need to set execution policy first (run PowerShell as Administrator)
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser

# Run the script
.\setup_node_red.ps1
```

### `remove-node-red.sh` (Linux/macOS)

This script removes Node-RED and its packages for debugging purposes.

#### Usage:

```bash
# Make sure the script is executable
chmod +x remove-node-red.sh

# Run the script
./remove-node-red.sh
```

### `remove-node-red.ps1` (Windows)

This is the Windows PowerShell version of the removal script.

#### Usage:

```powershell
# Run the script
.\remove-node-red.ps1
```

## After Setup

Once Node-RED is set up with the custom nodes:

1. Start Node-RED by running `node-red` in your terminal
2. Open http://localhost:1880 in your web browser
3. Design your network using the custom pipe nodes
4. Export your flow as JSON (Menu → Export → JSON)
5. Save the JSON file to the `Molecular Simulation/Config` directory
6. Use the `node-red-to-yaml.py` script to convert the JSON to YAML:
   ```bash
   python3 "Molecular Simulation/Config/node-red-to-yaml.py" "path/to/your/flow.json" "Molecular Simulation/Config/network_config.yaml"
   ```
7. Build and run the Molecular Simulation project
