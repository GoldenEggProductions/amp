Adaptive Machine Platform (AMP)
===============================

Intro
-----
AMP is an adaptive machine platform...

  
Installation
------------
    git clone https://github.com/GoldenEggProductions/amp.git ~/src/amp

LED Test Program
------------
test_led will flash the fault led (D16) at a 1Hz rate continuously.

    cd ~/src/amp 
    scons ld_led

Dip Switch Test Program
------------
test_dips will flash the fault led (D16) for a count equal to the value of the dip switches.

    cd ~/src/amp 
    scons ld_dips
