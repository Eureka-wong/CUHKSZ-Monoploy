# CUHKSZ Monopoly Online - 多人联机大富翁

### run client
   CUHKSZ-Monoploy-online\Client_Portable\CUHKSZ_Monopoly_Online.exe
   - default: `ws://10.30.198.46:9002`
   If you run your iwn server, you have to change the path.


# Rebuild client and run client
   # clone submodule
   git submodule init 
   git submodule update

   CUHKSZ-Monoploy-online\run.ps1
   You have to check your Qt version and path as well as your Visual Studio version and path in run.ps1.

# Generate new package:
   CUHKSZ-Monoploy-online\package_client.ps1
   You also have to check the path before you run this one.

### Rebuild and run server

1. server menu：
   ```powershell
   cd CUHKSZ-Monoploy-online\server
   ```

2. build server：
   ```powershell
   .\build.ps1
   ```
   You have to check your Visual Studio version and path in build.ps1.

3. start Gateway server：
   ```powershell
   .\start.ps1
   ```




