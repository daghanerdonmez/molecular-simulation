# setup_node_red.ps1 - Installs Node-RED and custom nodes for Molecular Simulation

# Text formatting
$ESC = [char]27
$BOLD = "$ESC[1m"
$GREEN = "$ESC[0;32m"
$YELLOW = "$ESC[0;33m"
$RED = "$ESC[0;31m"
$NC = "$ESC[0m" # No Color

# Print header
Write-Host "${BOLD}===============================================${NC}"
Write-Host "${BOLD}Molecular Simulation - Node-RED Setup${NC}"
Write-Host "${BOLD}===============================================${NC}"
Write-Host ""

# Function to print status messages
function Print-Status {
    param([string]$message)
    Write-Host "${YELLOW}[*] $message${NC}"
}

# Function to print success messages
function Print-Success {
    param([string]$message)
    Write-Host "${GREEN}[✓] $message${NC}"
}

# Function to print error messages
function Print-Error {
    param([string]$message)
    Write-Host "${RED}[✗] $message${NC}"
}

# Function to check if a command exists
function Command-Exists {
    param([string]$command)
    $exists = $null -ne (Get-Command $command -ErrorAction SilentlyContinue)
    return $exists
}

# Check for Node.js and npm
Print-Status "Checking for Node.js and npm..."
if (-not (Command-Exists "node") -or -not (Command-Exists "npm")) {
    Print-Error "Node.js and npm are required but not installed."
    Write-Host "Please install Node.js from https://nodejs.org/ (version 14.x or later recommended)"
    Write-Host "After installing Node.js, run this script again."
    exit 1
}
else {
    $NODE_VERSION = (node -v)
    $NPM_VERSION = (npm -v)
    Print-Success "Found Node.js $NODE_VERSION and npm $NPM_VERSION"
}

# Check for Node-RED
Print-Status "Checking for Node-RED..."
if (-not (Command-Exists "node-red")) {
    Print-Status "Node-RED not found. Installing globally..."
    npm install -g --unsafe-perm node-red
    if ($LASTEXITCODE -ne 0) {
        Print-Error "Failed to install Node-RED. Please check the error messages above."
        Write-Host "Try running PowerShell as Administrator if you encounter permission issues."
        exit 1
    }
    Print-Success "Node-RED installed successfully"
}
else {
    $NODE_RED_VERSION = (node-red --version)
    Print-Success "Found Node-RED $NODE_RED_VERSION"
}

# Ensure Node-RED user directory exists
$NODE_RED_DIR = "$env:APPDATA\npm\node_modules\node-red\.node-red"
if (-not (Test-Path $NODE_RED_DIR)) {
    Print-Status "Creating Node-RED user directory at $NODE_RED_DIR..."
    New-Item -ItemType Directory -Path $NODE_RED_DIR -Force | Out-Null
}

# Install custom nodes
Print-Status "Installing custom Node-RED nodes for Molecular Simulation..."

# Check if git is installed
if (-not (Command-Exists "git")) {
    Print-Error "Git is required but not installed."
    Write-Host "Please install Git from https://git-scm.com/ and run this script again."
    exit 1
}

# Create a temporary directory for the custom nodes
$TEMP_DIR = [System.IO.Path]::GetTempPath() + [System.Guid]::NewGuid().ToString()
New-Item -ItemType Directory -Path $TEMP_DIR -Force | Out-Null
Print-Status "Cloning custom nodes repository to temporary directory..."

# Clone the repository
git clone https://github.com/daghanerdonmez/custom-node-red-nodes.git $TEMP_DIR
if ($LASTEXITCODE -ne 0) {
    Print-Error "Failed to clone custom nodes repository. Please check the error messages above."
    Remove-Item -Path $TEMP_DIR -Recurse -Force -ErrorAction SilentlyContinue
    exit 1
}

# Install the custom nodes to Node-RED
Print-Status "Installing custom nodes to Node-RED..."
Push-Location $NODE_RED_DIR
npm install "$TEMP_DIR\node-red-contrib-pipe"
if ($LASTEXITCODE -ne 0) {
    Print-Error "Failed to install custom nodes. Please check the error messages above."
    Pop-Location
    Remove-Item -Path $TEMP_DIR -Recurse -Force -ErrorAction SilentlyContinue
    exit 1
}
Pop-Location

# Clean up
Remove-Item -Path $TEMP_DIR -Recurse -Force -ErrorAction SilentlyContinue
Print-Success "Custom nodes installed successfully"

# Final instructions
Write-Host ""
Write-Host "${BOLD}===============================================${NC}"
Write-Host "${BOLD}Node-RED Setup Complete!${NC}"
Write-Host "${BOLD}===============================================${NC}"
Write-Host ""
Write-Host "To start Node-RED, run:"
Write-Host "  node-red"
Write-Host ""
Write-Host "Then open http://localhost:1880 in your web browser"
Write-Host ""
Write-Host "You can now use the custom pipe nodes to create your network."
Write-Host "After designing your network:"
Write-Host "1. Export the flow as JSON (Menu -> Export -> JSON)"
Write-Host "2. Save the JSON file to the 'Molecular Simulation/Config' directory"
Write-Host "3. Use the node-red-to-yaml.py script to convert it to YAML"
Write-Host ""
