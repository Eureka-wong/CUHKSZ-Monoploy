# CUHKSZ Monopoly Online - 多人联机大富翁
 ```powershell
   cd CUHKSZ-Monoploy-online
   ```
## Run client (If you only want to play this game, you only need Client_Portable)
   packaged
   ```powershell
   .\Client_Portable\CUHKSZ_Monopoly_Online.exe
   ```
   or (if you have built from .\run.ps1 )
   ```powershell
   .\build\Windows-Release\Release\CUHKSZ_Monopoly_Online.exe
   ```
   - default: `ws://10.30.198.46:9002`
   If you run your own server, you have to change the path.


## Build(rebuild) client and run client
   ### clone submodule
   ```powershell
   git submodule init 
   git submodule update
   ```
   ### build/rebuild
   ```powershell
   .\run.ps1
   ```
   You have to check your Qt version and path as well as your Visual Studio version and path in run.ps1.

## Generate new package:
   ```powershell
   .\package_client.ps1
   ```
   You also have to check the path before you run this one.

## Rebuild and run server

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




