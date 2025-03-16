#!/bin/bash
# remove-node-red.sh - Removes Node-RED and its packages for debugging purposes

# Text formatting
BOLD='\033[1m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Print header
echo -e "${BOLD}===============================================${NC}"
echo -e "${BOLD}Node-RED Removal Script${NC}"
echo -e "${BOLD}===============================================${NC}"
echo ""

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

# Function to check if a command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Check if Node-RED is installed
if ! command_exists node-red; then
    print_status "Node-RED is not installed globally."
else
    print_status "Removing global Node-RED installation..."
    npm uninstall -g --unsafe-perm node-red
    if [ $? -ne 0 ]; then
        print_error "Failed to uninstall Node-RED globally. You might need to use sudo."
        echo "Try running: sudo npm uninstall -g --unsafe-perm node-red"
    else
        print_success "Node-RED has been uninstalled globally"
    fi
fi

# Remove Node-RED user directory
NODE_RED_DIR="$HOME/.node-red"
if [ -d "$NODE_RED_DIR" ]; then
    print_status "Removing Node-RED user directory at $NODE_RED_DIR..."
    
    # Ask for confirmation
    echo -e "${RED}WARNING: This will delete all your Node-RED flows and custom nodes${NC}"
    read -p "Are you sure you want to continue? (y/n): " -n 1 -r
    echo
    
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        rm -rf "$NODE_RED_DIR"
        if [ $? -ne 0 ]; then
            print_error "Failed to remove Node-RED user directory"
        else
            print_success "Node-RED user directory has been removed"
        fi
    else
        print_status "Skipping removal of Node-RED user directory"
    fi
else
    print_status "Node-RED user directory not found at $NODE_RED_DIR"
fi

# Check for and remove any remaining Node-RED modules in npm global space
print_status "Checking for remaining Node-RED modules..."
NODE_RED_MODULES=$(npm list -g | grep node-red)

if [ -n "$NODE_RED_MODULES" ]; then
    print_status "Found the following Node-RED modules:"
    echo "$NODE_RED_MODULES"
    
    read -p "Do you want to remove all these modules? (y/n): " -n 1 -r
    echo
    
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        # Extract module names and uninstall them
        for MODULE in $(echo "$NODE_RED_MODULES" | grep -o "node-red[^ ]*"); do
            print_status "Removing $MODULE..."
            npm uninstall -g "$MODULE"
        done
        print_success "Removed remaining Node-RED modules"
    else
        print_status "Skipping removal of remaining Node-RED modules"
    fi
else
    print_status "No remaining Node-RED modules found in global npm space"
fi

# Check for Node-RED processes and kill them
if pgrep -f "node-red" > /dev/null; then
    print_status "Node-RED processes are still running. Attempting to kill them..."
    pkill -f "node-red"
    sleep 1
    
    if pgrep -f "node-red" > /dev/null; then
        print_error "Failed to kill all Node-RED processes. You may need to kill them manually."
        echo "Try running: pkill -9 -f \"node-red\""
    else
        print_success "All Node-RED processes have been terminated"
    fi
else
    print_status "No running Node-RED processes found"
fi

# Final message
echo ""
echo -e "${BOLD}===============================================${NC}"
echo -e "${BOLD}Node-RED Removal Complete${NC}"
echo -e "${BOLD}===============================================${NC}"
echo ""
echo "Node-RED and its components have been removed from your system."
echo "To reinstall Node-RED, run the setup_node_red.sh script."
echo ""
