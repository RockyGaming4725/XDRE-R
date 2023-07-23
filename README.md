# XDRE

## To Do
(none) - next release (hopefully), (?) - Unsure

- Add Impse Glider to tools
- Support linedef brute forcing (?)
- Update PrBoom+ version (?)

## Releases
[+] — added, [-] — removed, [\*] — fixed/changed.

### 2.23 (20.07.23)
[+] Added autorange feature to brute force. (almostmatt1)
[+] Added toggles for showing/hiding thing/linedef/sector tracking.
[+] Palette stuff and tracking toggles are saved to config after quit.

[\*] Locks brute force check values to 6 digits past decimal. Fixes "=" brute 
     forcing when looking for a non-integer.

[\*] Brute force gui completely reworked. (almostmatt1 & rocky4725)
[\*] Brute force ticlist now pushes tics backward (almostmatt1). Is toggleable.
[\*] Fixed some GUI misalignments.
[\*] Fixed a bug where XDRE would launch with a larger window size than it should.
[\*] "Clear all traces" puts default value of zero after usage, staying in  line
      with xdre's default values when launched.
