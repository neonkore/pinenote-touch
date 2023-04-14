# Pine64 PineNote cyttsp5 firmware and configuration modifications

## Motivation
The Pine64 PineNote touch panel and controller support up to ten finger input. However, the combination of vendor firmware and configration support only up to fingers, advertising `max_num_of_tch_per_refresh_cycle=2` in the sensing configuration data. While playing around with the vendor `cyttsp5` driver trying to dump the configuration I accidentally overwrote the firmware, leaving me with no touch input at all. Using another firmware image and creating my own configuration I not only got touch input back, but can also use up to ten fingers with https://github.com/m-weigand/linux/pull/3.

## Background
- Touch controller: `CYTM448-70BUI`
  - Incomplete datasheet: http://updates.cypress.com/updates/truetouchhostemulator/3.3/_3.3_ReleaseNotes.pdf
    - Contains pointers where to find proprietary software that includes firmware files (`CYTMA448`, `2.0.634371` and `2.0.724515`) 
  - Firmware version: 2.0.827952: firmware revision not included in above software packag
  - The firmware is probably flashed at the factory and not obtainable by the Pine64 community
  - Flashing the 2.0.724515 firmware with dummy configuration allows the input device to be probed again, but the missing configuration causes the coordinates and touch sensitivity to be messed up, among others ...
  - ... so I created my own configuration for the 2.0.724515 using a dump and size information obtained from the vendor kernel

## Flash firmware and configuration
** DISCLAIMER: this may break your device. Do this on your own responsibility. **
- Flash `CYTMA448 2.0.724515` firmware (`cyttsp5_fw_2.0.724515.bin`)
- Flash a compatible configuration (`config_2.0.724515_v1.bin`)

## Outlook: modify configuration of vendor firmware
There may be a way to enable ten finger touch input using the original firmware, e.g. by setting one of the undocumented `0x02` bytes to `0x0a` or `0x00`.
** UNTESTED **
- [X] Create suitable version header `fw_vendor_version_header` (16 bytes). Contains firmware version information. I'm not sure what the last bytes are for or if they are even relevant. They are not included in the CRC.
- [X] Create configuration `cat fw_version_header_length fw_vendor_version_header param_regs.bin > config.bin`
- [ ] Upload configuration
- [ ] When modifying, recompute the CRC (`boost::crc_ccitt_false_t`).
- `config.bin`: hopefully flashable configuration file for the vendor `2.0.827952` firmware
- `config_to00.bin`: undocumented `0x02` byte at `0x53` is set to `0x00`. Not tested
- `config_to0a.bin`: undocumented `0x02` byte at `0x53` is set to `0x0a`. Not tested

## Sensing configuration reported by vendor firmware with orignal configuration

```
cyttsp5_pr_buf:  sysinfo_cydata[0..27]=01 02 0A 00 02 00 30 A2 0C 00 0D 00 02 00 9C 13 13 0C C0 CB 49 0C 6F 10 05 00 1E 03
cyttsp5_pr_buf:  sensing_conf_data[0..17]=1C 15 18 33 72 51 50 07 7C 05 FF 00 00 01 02 00 03 02
cyttsp5_si_put_log_data: pip_ver_major =0x01 (1)
cyttsp5_si_put_log_data: pip_ver_minor =0x02 (2)
cyttsp5_si_put_log_data: fw_pid =0x000A (10)
cyttsp5_si_put_log_data: fw_ver_major =0x02 (2)
cyttsp5_si_put_log_data: fw_ver_minor =0x00 (0)
cyttsp5_si_put_log_data: revctrl =0x000CA230 (827952)
cyttsp5_si_put_log_data: fw_ver_conf =0x000D (13)
cyttsp5_si_put_log_data: bl_ver_major =0x02 (2)
cyttsp5_si_put_log_data: bl_ver_minor =0x00 (0)
cyttsp5_si_put_log_data: jtag_id_h =0x0C13 (3091)
cyttsp5_si_put_log_data: jtag_id_l =0x139C (5020)
cyttsp5_si_put_log_data: mfg_id[0] =0xC0 (192)
cyttsp5_si_put_log_data: mfg_id[1] =0xCB (203)
cyttsp5_si_put_log_data: mfg_id[2] =0x49 (73)
cyttsp5_si_put_log_data: mfg_id[3] =0x0C (12)
cyttsp5_si_put_log_data: mfg_id[4] =0x6F (111)
cyttsp5_si_put_log_data: mfg_id[5] =0x10 (16)
cyttsp5_si_put_log_data: mfg_id[6] =0x05 (5)
cyttsp5_si_put_log_data: mfg_id[7] =0x00 (0)
cyttsp5_si_put_log_data: post_code =0x031E (798)
cyttsp5_si_put_log_data: electrodes_x =0x1C (28)
cyttsp5_si_put_log_data: electrodes_y =0x15 (21)
cyttsp5_si_put_log_data: len_x =0x3318 (13080)
cyttsp5_si_put_log_data: len_y =0x5172 (20850)
cyttsp5_si_put_log_data: res_x =0x0750 (1872)
cyttsp5_si_put_log_data: res_y =0x057C (1404)
cyttsp5_si_put_log_data: max_z =0x00FF (255)
cyttsp5_si_put_log_data: origin_x =0x00 (0)
cyttsp5_si_put_log_data: origin_y =0x01 (1)
cyttsp5_si_put_log_data: panel_id =0x02 (2)
cyttsp5_si_put_log_data: btn =0x00 (0)
cyttsp5_si_put_log_data: scan_mode =0x03 (3)
cyttsp5_si_put_log_data: max_num_of_tch_per_refresh_cycle =0x02 (2)
cyttsp5_si_put_log_data: xy_mode =00000000f6487179
cyttsp5_si_put_log_data: xy_data =00000000cc26aec8
```

## Sensing configuration obtained with 2.0.724515 firmware and modified configuration
Differences: `revctrl`, `max_num_of_tch_per_refresh_cycle`, `panel_id`, as well as `xy_mode` and `xy_data`. I don't know what the latter do.

```
cyttsp5_pr_buf:  sysinfo_cydata[0..27]=01 02 0A 00 02 00 23 0E 0B 00 0D 00 02 00 9C 13 13 0C C0 CB 49 0C 6F 10 05 00 1E 03
cyttsp5_pr_buf:  sensing_conf_data[0..17]=1C 15 18 33 72 51 50 07 7C 05 FF 00 00 01 01 00 03 0A
cyttsp5_si_put_log_data: pip_ver_major =0x01 (1)
cyttsp5_si_put_log_data: pip_ver_minor =0x02 (2)
cyttsp5_si_put_log_data: fw_pid =0x000A (10)
cyttsp5_si_put_log_data: fw_ver_major =0x02 (2)
cyttsp5_si_put_log_data: fw_ver_minor =0x00 (0)
cyttsp5_si_put_log_data: revctrl =0x000B0E23 (724515)
cyttsp5_si_put_log_data: fw_ver_conf =0x000D (13)
cyttsp5_si_put_log_data: bl_ver_major =0x02 (2)
cyttsp5_si_put_log_data: bl_ver_minor =0x00 (0)
cyttsp5_si_put_log_data: jtag_id_h =0x0C13 (3091)
cyttsp5_si_put_log_data: jtag_id_l =0x139C (5020)
cyttsp5_si_put_log_data: mfg_id[0] =0xC0 (192)
cyttsp5_si_put_log_data: mfg_id[1] =0xCB (203)
cyttsp5_si_put_log_data: mfg_id[2] =0x49 (73)
cyttsp5_si_put_log_data: mfg_id[3] =0x0C (12)
cyttsp5_si_put_log_data: mfg_id[4] =0x6F (111)
cyttsp5_si_put_log_data: mfg_id[5] =0x10 (16)
cyttsp5_si_put_log_data: mfg_id[6] =0x05 (5)
cyttsp5_si_put_log_data: mfg_id[7] =0x00 (0)
cyttsp5_si_put_log_data: post_code =0x031E (798)
cyttsp5_si_put_log_data: electrodes_x =0x1C (28)
cyttsp5_si_put_log_data: electrodes_y =0x15 (21)
cyttsp5_si_put_log_data: len_x =0x3318 (13080)
cyttsp5_si_put_log_data: len_y =0x5172 (20850)
cyttsp5_si_put_log_data: res_x =0x0750 (1872)
cyttsp5_si_put_log_data: res_y =0x057C (1404)
cyttsp5_si_put_log_data: max_z =0x00FF (255)
cyttsp5_si_put_log_data: origin_x =0x00 (0)
cyttsp5_si_put_log_data: origin_y =0x01 (1)
cyttsp5_si_put_log_data: panel_id =0x01 (1)
cyttsp5_si_put_log_data: btn =0x00 (0)
cyttsp5_si_put_log_data: scan_mode =0x03 (3)
cyttsp5_si_put_log_data: max_num_of_tch_per_refresh_cycle =0x0A (10)
cyttsp5_si_put_log_data: xy_mode =000000004f7da223
cyttsp5_si_put_log_data: xy_data =00000000af996865
```

# Flashing firmware and configuration

## Firmware flashing (2.0.724515)
Work in progress: https://github.com/hrdl-github/linux/tree/cyttsp5_loader

1. Build `cyttsp5_loader.ko` (and `cyttsp5_i2c.ko` as well as `cyttsp5.ko`) after enabling `CONFIG_TOUCHSCREEN_CYPRESS_CYTTSP5_BINARY_FW_UPGRADE=y`
2. Copy firmware `cyttsp5_fw_2.0.724515.bin` to `/lib/firmware/cyttp5_fw.bin`
3. Load modules `modprobe cyttsp5 cyttsp5_loader cyttsp5_i2c`
4. Watch output of `dmesg -w` or `journalctl -g cyttsp5` for errors
5. Once complete, unload loader module `modprobe -r cyttsp5_loader` and move firmware to prevent automatic loading `mv /lib/firmware/cyttp5_fw.bin /lib/firmware/cyttsp5_fw_2.0.724515.bin`

## Configuration flashing

1. Optionally modify `Driver.h`
2. Generate configuration: `g++ compile_config.cpp -o compile_config && ./compile_config config.bin`
3. Write configuration

```
sudo modprobe cyttsp5 cyttsp5_loader cyttsp5_i2c
echo 1 > /sys/bus/i2c/devices/5-0024/config_loading
cat config.bin > /sys/bus/i2c/devices/5-0024/config_data
echo 0 > /sys/bus/i2c/devices/5-0024/config_loading
```

When modifying `Driver.h`, recompiling should recompute the CRC field. When directly modifying the binary configuration file `vendor/update_crc.py` can be used to update the CRC and perform basic length checks. Verifying the configuration after upload should succeed (e.g. `cyttsp5_upgrade_ttconfig: CRC PASS, ebid=0, status=0, scrc=7EA6 ccrc=7EA6`).

To verify multi-touch support, run `sudo evtest |grep ABS_MT_SLOT` and keep track of the highest value. Alternatively inspect the sensing configuration data reported by the vendor driver, e.g. via `dmesg |grep max_num_of_tch_per_refresh_cycle` or `journalctl -b0 -g max_num_of_tch_per_refresh_cycle`.

# Original configuration
`vendor/annotated_dump` was an attempt to structure `vendor/param_regs.bin` based on `vendor/param_sizes.bin` while comparing it to the original 2.0.724515 `Driver.h`. `vendor/param_regs.bin` and `vendor/param_sizes.bin` were extracted from the vendor kernel image.

```
crc_length=2

sudo cat /dev/disk/by-partlabel/boot > boot.img
unpack_bootimg --boot_img boot.img --out .
sudo rm dtb ramdisk second boot.img
regs_addr=$((0x$(bgrep '18 33 72 51' kernel |cut -d: -f1 |head -n 1)-14))
sizes_addr=$(bgrep '02 00 02 00 04 00 02 00  01 00 01 00 01 00 01 00' kernel |cut -d: -f1 |head -n1)
regs_length=$(od -j "${regs_addr}" -N 2 kernel -d |head -n 1 |awk '{print $2}')
dd if=kernel of=param_regs.bin skip="${regs_addr}" ibs=1 count=$((regs_length+crc_length))
dd if=kernel of=param_sizes.bin skip="$((0x${sizes_addr}))" ibs=1 count=990
```

