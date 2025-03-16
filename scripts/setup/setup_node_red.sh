#!/bin/bash
# setup_node_red.sh - Installs Node-RED and custom nodes for Molecular Simulation

# Text formatting
BOLD='\033[1m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Enable debug mode
set -x

# Print header
echo -e "${BOLD}===============================================${NC}"
echo -e "${BOLD}Molecular Simulation - Node-RED Setup${NC}"
echo -e "${BOLD}===============================================${NC}"
echo ""

# Function to check if a command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Function to print status messages
print_status() {
    echo -e "${YELLOW}[*] $1${NC}"
}

# Function to print success messages
print_success() {
    echo -e "${GREEN}[✓] $1${NC}"
}

# Function to print error messages
print_error() {
    echo -e "${RED}[✗] $1${NC}"
}

# Check for Node.js and npm
print_status "Checking for Node.js and npm..."
if ! command_exists node || ! command_exists npm; then
    print_error "Node.js and npm are required but not installed."
    echo "Please install Node.js from https://nodejs.org/ (version 14.x or later recommended)"
    echo "After installing Node.js, run this script again."
    exit 1
else
    NODE_VERSION=$(node -v)
    NPM_VERSION=$(npm -v)
    print_success "Found Node.js $NODE_VERSION and npm $NPM_VERSION"
fi

# Check for Node-RED
print_status "Checking for Node-RED..."
if ! command_exists node-red; then
    print_status "Node-RED not found. Installing globally..."
    npm install -g --unsafe-perm node-red
    if [ $? -ne 0 ]; then
        print_error "Failed to install Node-RED. Please check the error messages above."
        exit 1
    fi
    print_success "Node-RED installed successfully"
else
    NODE_RED_VERSION=$(node-red --version)
    print_success "Found Node-RED $NODE_RED_VERSION"
fi

# Ensure Node-RED user directory exists
NODE_RED_DIR="$HOME/.node-red"
if [ ! -d "$NODE_RED_DIR" ]; then
    print_status "Creating Node-RED user directory at $NODE_RED_DIR..."
    mkdir -p "$NODE_RED_DIR"
fi

# Print current directory for debugging
print_status "Current directory before clone: $(pwd)"

# Install custom nodes
print_status "Installing custom Node-RED nodes for Molecular Simulation..."

# Check if git is installed
if ! command_exists git; then
    print_error "Git is required but not installed."
    echo "Please install Git and run this script again."
    exit 1
fi

# Create a temporary directory for the custom nodes
TEMP_DIR=$(mktemp -d)
print_status "Cloning custom nodes repository to temporary directory: $TEMP_DIR"

# Clone the repository
git clone https://github.com/daghanerdonmez/custom-node-red-nodes.git "$TEMP_DIR"
if [ $? -ne 0 ]; then
    print_error "Failed to clone custom nodes repository. Please check the error messages above."
    rm -rf "$TEMP_DIR"
    exit 1
fi

# List contents of the cloned repository for debugging
print_status "Contents of cloned repository:"
ls -la "$TEMP_DIR"
print_status "Contents of node-red-contrib-pipe directory:"
ls -la "$TEMP_DIR/node-red-contrib-pipe"

# Install the custom nodes to Node-RED
print_status "Changing to Node-RED directory: $NODE_RED_DIR"
cd "$NODE_RED_DIR" || exit 1
print_status "Current directory after cd: $(pwd)"

print_status "Installing custom nodes to Node-RED..."
npm install "$TEMP_DIR/node-red-contrib-pipe"
INSTALL_RESULT=$?

# Show npm list for debugging
print_status "Checking if node was installed:"
npm list | grep pipe

if [ $INSTALL_RESULT -ne 0 ]; then
    print_error "Failed to install custom nodes. Please check the error messages above."
    print_status "You can try installing manually with:"
    echo "  cd ~/.node-red"
    echo "  npm install /path/to/custom-node-red-nodes/node-red-contrib-pipe"
    rm -rf "$TEMP_DIR"
    exit 1
fi

# Clean up
rm -rf "$TEMP_DIR"
print_success "Custom nodes installed successfully"

# Disable debug mode
set +x

# Final instructions
echo ""
echo -e "${BOLD}===============================================${NC}"
echo -e "${BOLD}Node-RED Setup Complete!${NC}"
echo -e "${BOLD}===============================================${NC}"
echo ""
echo "To start Node-RED, run:"
echo "  node-red"
echo ""
echo "Then open http://localhost:1880 in your web browser"
echo ""
echo "You can now use the custom pipe nodes to create your network."
echo "After designing your network:"
echo "1. Export the flow as JSON (Menu -> Export -> JSON)"
echo "2. Save the JSON file to the 'Molecular Simulation/Config' directory"
echo "3. Use the node-red-to-yaml.py script to convert it to YAML"
echo ""
