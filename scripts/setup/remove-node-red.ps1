# remove-node-red.ps1 - Removes Node-RED and its packages for debugging purposes

# Text formatting
$ESC = [char]27
$BOLD = "$ESC[1m"
$GREEN = "$ESC[0;32m"
$YELLOW = "$ESC[0;33m"
$RED = "$ESC[0;31m"
$NC = "$ESC[0m" # No Color

# Print header
Write-Host "${BOLD}===============================================${NC}"
Write-Host "${BOLD}Node-RED Removal Script${NC}"
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

# Check if Node-RED is installed
if (-not (Command-Exists "node-red")) {
    Print-Status "Node-RED is not installed globally."
}
else {
    Print-Status "Removing global Node-RED installation..."
    npm uninstall -g --unsafe-perm node-red
    if ($LASTEXITCODE -ne 0) {
        Print-Error "Failed to uninstall Node-RED globally. You might need to run as Administrator."
        Write-Host "Try running PowerShell as Administrator and run this script again."
    }
    else {
        Print-Success "Node-RED has been uninstalled globally"
    }
}

# Remove Node-RED user directory
$NODE_RED_DIR = "$env:APPDATA\npm\node_modules\node-red\.node-red"
if (Test-Path $NODE_RED_DIR) {
    Print-Status "Found Node-RED user directory at $NODE_RED_DIR"
    
    # Ask for confirmation
    Write-Host "${RED}WARNING: This will delete all your Node-RED flows and custom nodes${NC}"
    $confirmation = Read-Host "Are you sure you want to continue? (y/n)"
    
    if ($confirmation -eq 'y' -or $confirmation -eq 'Y') {
        Remove-Item -Path $NODE_RED_DIR -Recurse -Force -ErrorAction SilentlyContinue
        if (Test-Path $NODE_RED_DIR) {
            Print-Error "Failed to remove Node-RED user directory"
        }
        else {
            Print-Success "Node-RED user directory has been removed"
        }
    }
    else {
        Print-Status "Skipping removal of Node-RED user directory"
    }
}
else {
    Print-Status "Node-RED user directory not found at $NODE_RED_DIR"
    
    # Check alternative location
    $ALT_NODE_RED_DIR = "$env:USERPROFILE\.node-red"
    if (Test-Path $ALT_NODE_RED_DIR) {
        Print-Status "Found Node-RED user directory at alternative location: $ALT_NODE_RED_DIR"
        
        # Ask for confirmation
        Write-Host "${RED}WARNING: This will delete all your Node-RED flows and custom nodes${NC}"
        $confirmation = Read-Host "Are you sure you want to continue? (y/n)"
        
        if ($confirmation -eq 'y' -or $confirmation -eq 'Y') {
            Remove-Item -Path $ALT_NODE_RED_DIR -Recurse -Force -ErrorAction SilentlyContinue
            if (Test-Path $ALT_NODE_RED_DIR) {
                Print-Error "Failed to remove Node-RED user directory"
            }
            else {
                Print-Success "Node-RED user directory has been removed"
            }
        }
        else {
            Print-Status "Skipping removal of Node-RED user directory"
        }
    }
}

# Check for and remove any remaining Node-RED modules in npm global space
Print-Status "Checking for remaining Node-RED modules..."
$NODE_RED_MODULES = npm list -g | Select-String "node-red"

if ($NODE_RED_MODULES) {
    Print-Status "Found the following Node-RED modules:"
    Write-Host $NODE_RED_MODULES
    
    $confirmation = Read-Host "Do you want to remove all these modules? (y/n)"
    
    if ($confirmation -eq 'y' -or $confirmation -eq 'Y') {
        # Extract module names and uninstall them
        foreach ($line in $NODE_RED_MODULES) {
            if ($line -match "node-red[^ ]*") {
                $module = $matches[0]
                Print-Status "Removing $module..."
                npm uninstall -g $module
            }
        }
        Print-Success "Removed remaining Node-RED modules"
    }
    else {
        Print-Status "Skipping removal of remaining Node-RED modules"
    }
}
else {
    Print-Status "No remaining Node-RED modules found in global npm space"
}

# Check for Node-RED processes and kill them
$nodeRedProcesses = Get-Process -Name "node" -ErrorAction SilentlyContinue | Where-Object { $_.CommandLine -match "node-red" }
if ($nodeRedProcesses) {
    Print-Status "Node-RED processes are still running. Attempting to kill them..."
    $nodeRedProcesses | ForEach-Object { Stop-Process -Id $_.Id -Force }
    Start-Sleep -Seconds 1
    
    $nodeRedProcesses = Get-Process -Name "node" -ErrorAction SilentlyContinue | Where-Object { $_.CommandLine -match "node-red" }
    if ($nodeRedProcesses) {
        Print-Error "Failed to kill all Node-RED processes. You may need to kill them manually."
        Write-Host "Try using Task Manager to end the Node-RED processes."
    }
    else {
        Print-Success "All Node-RED processes have been terminated"
    }
}
else {
    Print-Status "No running Node-RED processes found"
}

# Final message
Write-Host ""
Write-Host "${BOLD}===============================================${NC}"
Write-Host "${BOLD}Node-RED Removal Complete${NC}"
Write-Host "${BOLD}===============================================${NC}"
Write-Host ""
Write-Host "Node-RED and its components have been removed from your system."
Write-Host "To reinstall Node-RED, run the setup_node_red.ps1 script."
Write-Host ""
