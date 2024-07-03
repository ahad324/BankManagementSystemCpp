# Check current execution policy for the current user
$currentPolicy = Get-ExecutionPolicy -Scope CurrentUser

# If the current policy is not RemoteSigned, change it
if ($currentPolicy -ne 'RemoteSigned') {
    Write-Output "Current execution policy is $currentPolicy. Changing it to RemoteSigned."
    Set-ExecutionPolicy RemoteSigned -Scope CurrentUser -Force
    Write-Output "Execution policy has been set to RemoteSigned."
} else {
    Write-Output "Current execution policy is already RemoteSigned. No changes needed."
}
