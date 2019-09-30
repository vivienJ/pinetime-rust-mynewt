::  Flash Mynewt Application to nRF52 with ST-Link on Windows

::  Added "set WORKAREASIZE 0" to remove the warning: "using fast async flash loader. This is currently supported only with ST-Link and CMSIS-DAP"

openocd\bin\openocd.exe 
    -f scripts/nrf52/flash-init.ocd ^
    -f interface/stlink.cfg ^
    -c "transport select hla_swd" ^
    -c "set WORKAREASIZE 0" ^
    -f target/nrf52.cfg ^
    -f scripts/nrf52/flash-app.ocd
