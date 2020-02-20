EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 3
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel 5225 3875 0    50   Input ~ 0
SERIAL_TX2
Text HLabel 5225 3775 0    50   Input ~ 0
SERIAL_RX2
Text HLabel 5225 2675 0    50   Input ~ 0
ORDER_INTERRUPT_PIN
$Comp
L 1nofilterbooth:MEGA2560PRO-EMBED MEGA_PAPER?
U 1 1 5E40A8A4
P 5725 3275
AR Path="/5E40A8A4" Ref="MEGA_PAPER?"  Part="1" 
AR Path="/5E432787/5E40A8A4" Ref="MEGA_PAPER1"  Part="1" 
F 0 "MEGA_PAPER1" H 5725 5442 50  0000 C CNN
F 1 "MEGA2560PRO-EMBED" H 5725 5351 50  0000 C CNN
F 2 "1nofilterbooth:MEGA2560PRO-EMBED" V 5675 1875 50  0001 L BNN
F 3 "" H 5725 3275 50  0001 C CNN
	1    5725 3275
	1    0    0    -1  
$EndComp
Wire Wire Line
	1525 1550 1775 1550
Wire Wire Line
	1775 1550 1775 1500
Wire Wire Line
	1525 1650 1775 1650
Wire Wire Line
	1775 1650 1775 1600
Wire Wire Line
	1525 1750 1775 1750
Wire Wire Line
	1775 1750 1775 1800
Wire Wire Line
	1525 1850 1775 1850
Wire Wire Line
	1775 1850 1775 1900
$Comp
L 1nofilterbooth:Pololu_Breakout_DRV8825 DRV_SCISSOR?
U 1 1 5E42155C
P 2175 1800
AR Path="/5E3BF423/5E42155C" Ref="DRV_SCISSOR?"  Part="1" 
AR Path="/5E432787/5E42155C" Ref="DRV_ROT1"  Part="1" 
F 0 "DRV_ROT1" V 2275 1750 50  0000 C CNN
F 1 "Pololu_Breakout_DRV8825" V 2075 1750 50  0000 C CNN
F 2 "Module:Pololu_Breakout-16_15.2x20.3mm" H 2375 1000 50  0001 L CNN
F 3 "https://www.pololu.com/product/2982" H 2275 1500 50  0001 C CNN
	1    2175 1800
	-1   0    0    1   
$EndComp
$Comp
L 1nofilterbooth:GND-power #PWR0169
U 1 1 5E421562
P 2075 1000
F 0 "#PWR0169" H 2075 750 50  0001 C CNN
F 1 "GND" H 2080 827 50  0000 C CNN
F 2 "" H 2075 1000 50  0001 C CNN
F 3 "" H 2075 1000 50  0001 C CNN
	1    2075 1000
	-1   0    0    1   
$EndComp
$Comp
L 1nofilterbooth:GND-power #PWR0170
U 1 1 5E421568
P 2175 1000
F 0 "#PWR0170" H 2175 750 50  0001 C CNN
F 1 "GND" H 2180 827 50  0000 C CNN
F 2 "" H 2175 1000 50  0001 C CNN
F 3 "" H 2175 1000 50  0001 C CNN
	1    2175 1000
	-1   0    0    1   
$EndComp
NoConn ~ 2575 2200
$Comp
L 1nofilterbooth:Conn_01x04-Connector_Generic CONN_STEP_SCISSOR?
U 1 1 5E421575
P 1325 1750
AR Path="/5E3BF423/5E421575" Ref="CONN_STEP_SCISSOR?"  Part="1" 
AR Path="/5E432787/5E421575" Ref="CONN_STEP_ROT1"  Part="1" 
F 0 "CONN_STEP_ROT1" H 1243 1325 50  0000 C CNN
F 1 "JST_XH4" H 1243 1416 50  0000 C CNN
F 2 "Connector_JST:JST_XH_S4B-XH-A-1_1x04_P2.50mm_Horizontal" H 1325 1750 50  0001 C CNN
F 3 "~" H 1325 1750 50  0001 C CNN
	1    1325 1750
	-1   0    0    1   
$EndComp
Text GLabel 2575 1400 2    50   Input ~ 0
ROT_M0
Wire Wire Line
	2575 1300 2575 1200
Wire Wire Line
	2575 1000 2175 1000
Connection ~ 2575 1200
Wire Wire Line
	2575 1200 2575 1000
Connection ~ 2075 1000
Connection ~ 2175 1000
Wire Wire Line
	2175 1000 2075 1000
Wire Wire Line
	2175 2400 2525 2400
$Comp
L 1nofilterbooth:CP-Device C?
U 1 1 5E421590
P 2525 2550
AR Path="/5E3BF423/5E421590" Ref="C?"  Part="1" 
AR Path="/5E432787/5E421590" Ref="C_ROT1"  Part="1" 
F 0 "C_ROT1" H 2643 2596 50  0000 L CNN
F 1 "100uF" H 2643 2505 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D6.3mm_P2.50mm" H 2563 2400 50  0001 C CNN
F 3 "~" H 2525 2550 50  0001 C CNN
	1    2525 2550
	1    0    0    -1  
$EndComp
Connection ~ 2525 2400
Wire Wire Line
	2525 2400 2675 2400
Text GLabel 2575 1600 2    50   Input ~ 0
SPIDER_ROTATE_PIN_DIR
Text GLabel 2575 1700 2    50   Input ~ 0
SPIDER_ROTATE_PIN_STP
Text GLabel 2575 1800 2    50   Input ~ 0
SPIDER_ROTATE_PIN_ENABLE
$Comp
L 1nofilterbooth:Cytron_MD10C DRV_DC1
U 1 1 5E42C126
P 1925 5800
F 0 "DRV_DC1" H 2262 5965 50  0000 C CNN
F 1 "Cytron_MD10C" H 2262 5874 50  0000 C CNN
F 2 "1nofilterbooth:CYTRON_MD10C" H 1925 5800 50  0001 C CNN
F 3 "" H 1925 5800 50  0001 C CNN
	1    1925 5800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0173
U 1 1 5E438D17
P 2525 2700
F 0 "#PWR0173" H 2525 2450 50  0001 C CNN
F 1 "GND" H 2530 2527 50  0000 C CNN
F 2 "" H 2525 2700 50  0001 C CNN
F 3 "" H 2525 2700 50  0001 C CNN
	1    2525 2700
	1    0    0    -1  
$EndComp
$Comp
L power:+12V #PWR0174
U 1 1 5E4397FF
P 2675 2400
F 0 "#PWR0174" H 2675 2250 50  0001 C CNN
F 1 "+12V" V 2690 2528 50  0000 L CNN
F 2 "" H 2675 2400 50  0001 C CNN
F 3 "" H 2675 2400 50  0001 C CNN
	1    2675 2400
	0    1    1    0   
$EndComp
$Comp
L power:+5V #PWR0175
U 1 1 5E43A329
P 2575 2000
F 0 "#PWR0175" H 2575 1850 50  0001 C CNN
F 1 "+5V" V 2590 2128 50  0000 L CNN
F 2 "" H 2575 2000 50  0001 C CNN
F 3 "" H 2575 2000 50  0001 C CNN
	1    2575 2000
	0    1    1    0   
$EndComp
$Comp
L power:+5V #PWR0176
U 1 1 5E43AC9C
P 2575 2100
F 0 "#PWR0176" H 2575 1950 50  0001 C CNN
F 1 "+5V" V 2590 2228 50  0000 L CNN
F 2 "" H 2575 2100 50  0001 C CNN
F 3 "" H 2575 2100 50  0001 C CNN
	1    2575 2100
	0    1    1    0   
$EndComp
Wire Wire Line
	1475 3700 1725 3700
Wire Wire Line
	1725 3700 1725 3650
Wire Wire Line
	1475 3800 1725 3800
Wire Wire Line
	1725 3800 1725 3750
Wire Wire Line
	1475 3900 1725 3900
Wire Wire Line
	1725 3900 1725 3950
Wire Wire Line
	1475 4000 1725 4000
Wire Wire Line
	1725 4000 1725 4050
$Comp
L 1nofilterbooth:Pololu_Breakout_DRV8825 DRV_SCISSOR?
U 1 1 5E43ED5B
P 2125 3950
AR Path="/5E3BF423/5E43ED5B" Ref="DRV_SCISSOR?"  Part="1" 
AR Path="/5E432787/5E43ED5B" Ref="DRV_EXIT1"  Part="1" 
F 0 "DRV_EXIT1" V 2225 3900 50  0000 C CNN
F 1 "Pololu_Breakout_DRV8825" V 2025 3900 50  0000 C CNN
F 2 "Module:Pololu_Breakout-16_15.2x20.3mm" H 2325 3150 50  0001 L CNN
F 3 "https://www.pololu.com/product/2982" H 2225 3650 50  0001 C CNN
	1    2125 3950
	-1   0    0    1   
$EndComp
$Comp
L 1nofilterbooth:GND-power #PWR0177
U 1 1 5E43ED61
P 2025 3150
F 0 "#PWR0177" H 2025 2900 50  0001 C CNN
F 1 "GND" H 2030 2977 50  0000 C CNN
F 2 "" H 2025 3150 50  0001 C CNN
F 3 "" H 2025 3150 50  0001 C CNN
	1    2025 3150
	-1   0    0    1   
$EndComp
$Comp
L 1nofilterbooth:GND-power #PWR0178
U 1 1 5E43ED67
P 2125 3150
F 0 "#PWR0178" H 2125 2900 50  0001 C CNN
F 1 "GND" H 2130 2977 50  0000 C CNN
F 2 "" H 2125 3150 50  0001 C CNN
F 3 "" H 2125 3150 50  0001 C CNN
	1    2125 3150
	-1   0    0    1   
$EndComp
NoConn ~ 2525 4350
$Comp
L 1nofilterbooth:Conn_01x04-Connector_Generic CONN_STEP_SCISSOR?
U 1 1 5E43ED6E
P 1275 3900
AR Path="/5E3BF423/5E43ED6E" Ref="CONN_STEP_SCISSOR?"  Part="1" 
AR Path="/5E432787/5E43ED6E" Ref="CONN_STEP_EXIT1"  Part="1" 
F 0 "CONN_STEP_EXIT1" H 1193 3475 50  0000 C CNN
F 1 "JST_XH4" H 1193 3566 50  0000 C CNN
F 2 "Connector_JST:JST_XH_S4B-XH-A-1_1x04_P2.50mm_Horizontal" H 1275 3900 50  0001 C CNN
F 3 "~" H 1275 3900 50  0001 C CNN
	1    1275 3900
	-1   0    0    1   
$EndComp
Text GLabel 2525 3550 2    50   Input ~ 0
EXIT_M0
Wire Wire Line
	2525 3450 2525 3350
Wire Wire Line
	2525 3150 2125 3150
Connection ~ 2525 3350
Wire Wire Line
	2525 3350 2525 3150
Connection ~ 2025 3150
Connection ~ 2125 3150
Wire Wire Line
	2125 3150 2025 3150
Text GLabel 2525 3750 2    50   Input ~ 0
SPIDER_EXIT_PIN_DIR
Text GLabel 2525 3850 2    50   Input ~ 0
SPIDER_EXIT_PIN_STP
Text GLabel 2525 3950 2    50   Input ~ 0
SPIDER_EXIT_PIN_ENABLE
$Comp
L power:GND #PWR0179
U 1 1 5E43ED88
P 2475 4850
F 0 "#PWR0179" H 2475 4600 50  0001 C CNN
F 1 "GND" H 2480 4677 50  0000 C CNN
F 2 "" H 2475 4850 50  0001 C CNN
F 3 "" H 2475 4850 50  0001 C CNN
	1    2475 4850
	1    0    0    -1  
$EndComp
$Comp
L power:+12V #PWR0180
U 1 1 5E43ED8E
P 2625 4550
F 0 "#PWR0180" H 2625 4400 50  0001 C CNN
F 1 "+12V" V 2640 4678 50  0000 L CNN
F 2 "" H 2625 4550 50  0001 C CNN
F 3 "" H 2625 4550 50  0001 C CNN
	1    2625 4550
	0    1    1    0   
$EndComp
$Comp
L power:+5V #PWR0181
U 1 1 5E43ED94
P 2525 4150
F 0 "#PWR0181" H 2525 4000 50  0001 C CNN
F 1 "+5V" V 2540 4278 50  0000 L CNN
F 2 "" H 2525 4150 50  0001 C CNN
F 3 "" H 2525 4150 50  0001 C CNN
	1    2525 4150
	0    1    1    0   
$EndComp
$Comp
L power:+5V #PWR0182
U 1 1 5E43ED9A
P 2525 4250
F 0 "#PWR0182" H 2525 4100 50  0001 C CNN
F 1 "+5V" V 2540 4378 50  0000 L CNN
F 2 "" H 2525 4250 50  0001 C CNN
F 3 "" H 2525 4250 50  0001 C CNN
	1    2525 4250
	0    1    1    0   
$EndComp
Text GLabel 5225 2975 0    50   Input ~ 0
SPIDER_ROTATE_PIN_ENABLE
Text GLabel 5225 3075 0    50   Input ~ 0
ROT_M0
Text GLabel 5225 3275 0    50   Input ~ 0
SPIDER_ROTATE_PIN_DIR
Text GLabel 5225 3375 0    50   Input ~ 0
SPIDER_EXIT_PIN_ENABLE
Text GLabel 5225 3475 0    50   Input ~ 0
EXIT_M0
Text GLabel 5225 3575 0    50   Input ~ 0
SPIDER_EXIT_PIN_STP
Text GLabel 5225 3675 0    50   Input ~ 0
SPIDER_EXIT_PIN_DIR
$Comp
L power:+12V #PWR0183
U 1 1 5E442CD7
P 1825 5900
F 0 "#PWR0183" H 1825 5750 50  0001 C CNN
F 1 "+12V" V 1840 6028 50  0000 L CNN
F 2 "" H 1825 5900 50  0001 C CNN
F 3 "" H 1825 5900 50  0001 C CNN
	1    1825 5900
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR0184
U 1 1 5E4436E4
P 1825 6000
F 0 "#PWR0184" H 1825 5750 50  0001 C CNN
F 1 "GND" V 1830 5872 50  0000 R CNN
F 2 "" H 1825 6000 50  0001 C CNN
F 3 "" H 1825 6000 50  0001 C CNN
	1    1825 6000
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0185
U 1 1 5E443EEC
P 2700 6100
F 0 "#PWR0185" H 2700 5850 50  0001 C CNN
F 1 "GND" V 2705 5972 50  0000 R CNN
F 2 "" H 2700 6100 50  0001 C CNN
F 3 "" H 2700 6100 50  0001 C CNN
	1    2700 6100
	0    -1   -1   0   
$EndComp
Text GLabel 2700 5900 2    50   Input ~ 0
SPIDER_UPDOWN_PIN_DIR
Text GLabel 2700 6000 2    50   Input ~ 0
SPIDER_UPDOWN_PIN_PWM
Text GLabel 5225 2775 0    50   Input ~ 0
SPIDER_UPDOWN_PIN_DIR
Text GLabel 5225 2875 0    50   Input ~ 0
SPIDER_UPDOWN_PIN_PWM
Text GLabel 5225 3175 0    50   Input ~ 0
SPIDER_ROTATE_PIN_STP
$Comp
L 1nofilterbooth:+5V-power #PWR?
U 1 1 5E451673
P 9675 925
AR Path="/5E3BF423/5E451673" Ref="#PWR?"  Part="1" 
AR Path="/5E432787/5E451673" Ref="#PWR0186"  Part="1" 
F 0 "#PWR0186" H 9675 775 50  0001 C CNN
F 1 "+5V" V 9690 1053 50  0000 L CNN
F 2 "" H 9675 925 50  0001 C CNN
F 3 "" H 9675 925 50  0001 C CNN
	1    9675 925 
	0    1    1    0   
$EndComp
$Comp
L 1nofilterbooth:R_Small-Device R_SW_PAPER?
U 1 1 5E451679
P 9575 925
AR Path="/5E3BF423/5E451679" Ref="R_SW_PAPER?"  Part="1" 
AR Path="/5E432787/5E451679" Ref="R_SW_ROT1"  Part="1" 
F 0 "R_SW_ROT1" V 9379 925 50  0000 C CNN
F 1 "560ohm" V 9470 925 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" H 9575 925 50  0001 C CNN
F 3 "~" H 9575 925 50  0001 C CNN
	1    9575 925 
	0    1    1    0   
$EndComp
Wire Wire Line
	9475 1125 9175 1125
Connection ~ 9475 1125
$Comp
L 1nofilterbooth:LED_Small-Device LED_SW_PAPER?
U 1 1 5E451681
P 9475 1025
AR Path="/5E3BF423/5E451681" Ref="LED_SW_PAPER?"  Part="1" 
AR Path="/5E432787/5E451681" Ref="LED_SW_ROT1"  Part="1" 
F 0 "LED_SW_ROT1" V 9475 1725 50  0000 R CNN
F 1 "LED 3mm" V 9575 1525 50  0000 R CNN
F 2 "LED_THT:LED_D3.0mm" V 9475 1025 50  0001 C CNN
F 3 "~" V 9475 1025 50  0001 C CNN
	1    9475 1025
	0    -1   -1   0   
$EndComp
Text GLabel 9175 1125 0    50   Input ~ 0
SPIDER_ROTATE_ENDSTOP1_PIN
Wire Wire Line
	9975 1125 9475 1125
$Comp
L 1nofilterbooth:+5V-power #PWR?
U 1 1 5E451689
P 9975 1225
AR Path="/5E3BF423/5E451689" Ref="#PWR?"  Part="1" 
AR Path="/5E432787/5E451689" Ref="#PWR0187"  Part="1" 
F 0 "#PWR0187" H 9975 1075 50  0001 C CNN
F 1 "+5V" V 9990 1353 50  0000 L CNN
F 2 "" H 9975 1225 50  0001 C CNN
F 3 "" H 9975 1225 50  0001 C CNN
	1    9975 1225
	0    -1   -1   0   
$EndComp
$Comp
L 1nofilterbooth:GND-power #PWR?
U 1 1 5E45168F
P 9975 1025
AR Path="/5E3BF423/5E45168F" Ref="#PWR?"  Part="1" 
AR Path="/5E432787/5E45168F" Ref="#PWR0188"  Part="1" 
F 0 "#PWR0188" H 9975 775 50  0001 C CNN
F 1 "GND" V 9980 897 50  0000 R CNN
F 2 "" H 9975 1025 50  0001 C CNN
F 3 "" H 9975 1025 50  0001 C CNN
	1    9975 1025
	0    1    1    0   
$EndComp
$Comp
L 1nofilterbooth:+5V-power #PWR?
U 1 1 5E453001
P 9625 1775
AR Path="/5E3BF423/5E453001" Ref="#PWR?"  Part="1" 
AR Path="/5E432787/5E453001" Ref="#PWR0189"  Part="1" 
F 0 "#PWR0189" H 9625 1625 50  0001 C CNN
F 1 "+5V" V 9640 1903 50  0000 L CNN
F 2 "" H 9625 1775 50  0001 C CNN
F 3 "" H 9625 1775 50  0001 C CNN
	1    9625 1775
	0    1    1    0   
$EndComp
$Comp
L 1nofilterbooth:R_Small-Device R_SW_PAPER?
U 1 1 5E453007
P 9525 1775
AR Path="/5E3BF423/5E453007" Ref="R_SW_PAPER?"  Part="1" 
AR Path="/5E432787/5E453007" Ref="R_SW_ROT2"  Part="1" 
F 0 "R_SW_ROT2" V 9329 1775 50  0000 C CNN
F 1 "560ohm" V 9420 1775 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" H 9525 1775 50  0001 C CNN
F 3 "~" H 9525 1775 50  0001 C CNN
	1    9525 1775
	0    1    1    0   
$EndComp
Wire Wire Line
	9425 1975 9125 1975
Connection ~ 9425 1975
$Comp
L 1nofilterbooth:LED_Small-Device LED_SW_PAPER?
U 1 1 5E45300F
P 9425 1875
AR Path="/5E3BF423/5E45300F" Ref="LED_SW_PAPER?"  Part="1" 
AR Path="/5E432787/5E45300F" Ref="LED_SW_ROT2"  Part="1" 
F 0 "LED_SW_ROT2" V 9425 2575 50  0000 R CNN
F 1 "LED 3mm" V 9525 2375 50  0000 R CNN
F 2 "LED_THT:LED_D3.0mm" V 9425 1875 50  0001 C CNN
F 3 "~" V 9425 1875 50  0001 C CNN
	1    9425 1875
	0    -1   -1   0   
$EndComp
Text GLabel 9125 1975 0    50   Input ~ 0
SPIDER_ROTATE_ENDSTOP2_PIN
Wire Wire Line
	9925 1975 9425 1975
$Comp
L 1nofilterbooth:+5V-power #PWR?
U 1 1 5E453017
P 9925 2075
AR Path="/5E3BF423/5E453017" Ref="#PWR?"  Part="1" 
AR Path="/5E432787/5E453017" Ref="#PWR0190"  Part="1" 
F 0 "#PWR0190" H 9925 1925 50  0001 C CNN
F 1 "+5V" V 9940 2203 50  0000 L CNN
F 2 "" H 9925 2075 50  0001 C CNN
F 3 "" H 9925 2075 50  0001 C CNN
	1    9925 2075
	0    -1   -1   0   
$EndComp
$Comp
L 1nofilterbooth:GND-power #PWR?
U 1 1 5E45301D
P 9925 1875
AR Path="/5E3BF423/5E45301D" Ref="#PWR?"  Part="1" 
AR Path="/5E432787/5E45301D" Ref="#PWR0191"  Part="1" 
F 0 "#PWR0191" H 9925 1625 50  0001 C CNN
F 1 "GND" V 9930 1747 50  0000 R CNN
F 2 "" H 9925 1875 50  0001 C CNN
F 3 "" H 9925 1875 50  0001 C CNN
	1    9925 1875
	0    1    1    0   
$EndComp
$Comp
L 1nofilterbooth:Conn_01x03-Connector_Generic CONN_SW_PAP?
U 1 1 5E453023
P 10125 1975
AR Path="/5E3BF423/5E453023" Ref="CONN_SW_PAP?"  Part="1" 
AR Path="/5E432787/5E453023" Ref="CONN_SW_ROT2"  Part="1" 
F 0 "CONN_SW_ROT2" H 10205 2017 50  0000 L CNN
F 1 "JST_XH3" H 10205 1926 50  0000 L CNN
F 2 "Connector_JST:JST_XH_S3B-XH-A-1_1x03_P2.50mm_Horizontal" H 10125 1975 50  0001 C CNN
F 3 "~" H 10125 1975 50  0001 C CNN
	1    10125 1975
	1    0    0    -1  
$EndComp
Text GLabel 6225 3175 2    50   Input ~ 0
SPIDER_ROTATE_ENDSTOP1_PIN
Text GLabel 6225 3275 2    50   Input ~ 0
SPIDER_ROTATE_ENDSTOP2_PIN
$Comp
L 1nofilterbooth:Conn_01x03-Connector_Generic CONN_SW_PAP?
U 1 1 5E451695
P 10175 1125
AR Path="/5E3BF423/5E451695" Ref="CONN_SW_PAP?"  Part="1" 
AR Path="/5E432787/5E451695" Ref="CONN_SW_ROT1"  Part="1" 
F 0 "CONN_SW_ROT1" H 10255 1167 50  0000 L CNN
F 1 "JST_XH3" H 10255 1076 50  0000 L CNN
F 2 "Connector_JST:JST_XH_S3B-XH-A-1_1x03_P2.50mm_Horizontal" H 10175 1125 50  0001 C CNN
F 3 "~" H 10175 1125 50  0001 C CNN
	1    10175 1125
	1    0    0    -1  
$EndComp
$Comp
L 1nofilterbooth:GND-power #PWR?
U 1 1 5E459508
P 9900 3025
AR Path="/5E3BF423/5E459508" Ref="#PWR?"  Part="1" 
AR Path="/5E432787/5E459508" Ref="#PWR0192"  Part="1" 
F 0 "#PWR0192" H 9900 2775 50  0001 C CNN
F 1 "GND" V 9905 2897 50  0000 R CNN
F 2 "" H 9900 3025 50  0001 C CNN
F 3 "" H 9900 3025 50  0001 C CNN
	1    9900 3025
	0    1    1    0   
$EndComp
Wire Wire Line
	9300 2925 9600 2925
Text GLabel 9300 2925 0    50   Input ~ 0
SPIDER_UPDOWN_PIN_ENDSTOP_UP
$Comp
L 1nofilterbooth:Conn_01x02-Connector_Generic CONN_SW_SHUTTER?
U 1 1 5E459510
P 10100 2925
AR Path="/5E459510" Ref="CONN_SW_SHUTTER?"  Part="1" 
AR Path="/5E3BF423/5E459510" Ref="CONN_SW_SHUTTER?"  Part="1" 
AR Path="/5E432787/5E459510" Ref="CONN_SW_UP1"  Part="1" 
F 0 "CONN_SW_UP1" H 10180 2917 50  0000 L CNN
F 1 "JST_XH2" H 10180 2826 50  0000 L CNN
F 2 "Connector_JST:JST_XH_S2B-XH-A-1_1x02_P2.50mm_Horizontal" H 10100 2925 50  0001 C CNN
F 3 "~" H 10100 2925 50  0001 C CNN
	1    10100 2925
	1    0    0    -1  
$EndComp
$Comp
L 1nofilterbooth:LED_Small-Device LED_SW_SHUTTER?
U 1 1 5E459516
P 9600 2825
AR Path="/5E3BF423/5E459516" Ref="LED_SW_SHUTTER?"  Part="1" 
AR Path="/5E432787/5E459516" Ref="LED_SW_UP1"  Part="1" 
F 0 "LED_SW_UP1" V 9600 3350 50  0000 R CNN
F 1 "LED 3mm" V 9675 3200 50  0000 R CNN
F 2 "LED_THT:LED_D3.0mm" V 9600 2825 50  0001 C CNN
F 3 "~" V 9600 2825 50  0001 C CNN
	1    9600 2825
	0    -1   -1   0   
$EndComp
Connection ~ 9600 2925
Wire Wire Line
	9600 2925 9900 2925
$Comp
L 1nofilterbooth:R_Small-Device R_SW_SHUTTER?
U 1 1 5E45951E
P 9700 2725
AR Path="/5E3BF423/5E45951E" Ref="R_SW_SHUTTER?"  Part="1" 
AR Path="/5E432787/5E45951E" Ref="R_SW_UP1"  Part="1" 
F 0 "R_SW_UP1" V 9504 2725 50  0000 C CNN
F 1 "560ohm" V 9595 2725 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" H 9700 2725 50  0001 C CNN
F 3 "~" H 9700 2725 50  0001 C CNN
	1    9700 2725
	0    1    1    0   
$EndComp
$Comp
L 1nofilterbooth:+5V-power #PWR?
U 1 1 5E459524
P 9800 2725
AR Path="/5E3BF423/5E459524" Ref="#PWR?"  Part="1" 
AR Path="/5E432787/5E459524" Ref="#PWR0193"  Part="1" 
F 0 "#PWR0193" H 9800 2575 50  0001 C CNN
F 1 "+5V" V 9815 2853 50  0000 L CNN
F 2 "" H 9800 2725 50  0001 C CNN
F 3 "" H 9800 2725 50  0001 C CNN
	1    9800 2725
	0    1    1    0   
$EndComp
$Comp
L 1nofilterbooth:GND-power #PWR?
U 1 1 5E45F1F3
P 9975 4850
AR Path="/5E3BF423/5E45F1F3" Ref="#PWR?"  Part="1" 
AR Path="/5E432787/5E45F1F3" Ref="#PWR0196"  Part="1" 
F 0 "#PWR0196" H 9975 4600 50  0001 C CNN
F 1 "GND" V 9980 4722 50  0000 R CNN
F 2 "" H 9975 4850 50  0001 C CNN
F 3 "" H 9975 4850 50  0001 C CNN
	1    9975 4850
	0    1    1    0   
$EndComp
Wire Wire Line
	9375 4750 9675 4750
Text GLabel 9375 4750 0    50   Input ~ 0
SPIDER_UPDOWN_PIN_ENDSTOP_BOTTOM
$Comp
L 1nofilterbooth:Conn_01x02-Connector_Generic CONN_SW_UP?
U 1 1 5E45F1FF
P 10175 4750
AR Path="/5E45F1FF" Ref="CONN_SW_UP?"  Part="1" 
AR Path="/5E3BF423/5E45F1FF" Ref="CONN_SW_UP?"  Part="1" 
AR Path="/5E432787/5E45F1FF" Ref="CONN_SW_BOTTOM1"  Part="1" 
F 0 "CONN_SW_BOTTOM1" H 10255 4742 50  0000 L CNN
F 1 "JST_XH2" H 10255 4651 50  0000 L CNN
F 2 "Connector_JST:JST_XH_S2B-XH-A-1_1x02_P2.50mm_Horizontal" H 10175 4750 50  0001 C CNN
F 3 "~" H 10175 4750 50  0001 C CNN
	1    10175 4750
	1    0    0    -1  
$EndComp
$Comp
L 1nofilterbooth:LED_Small-Device LED_SW_UP?
U 1 1 5E45F209
P 9675 4650
AR Path="/5E3BF423/5E45F209" Ref="LED_SW_UP?"  Part="1" 
AR Path="/5E432787/5E45F209" Ref="LED_SW_BOTTOM1"  Part="1" 
F 0 "LED_SW_BOTTOM1" V 9675 5350 50  0000 R CNN
F 1 "LED 3mm" V 9750 5025 50  0000 R CNN
F 2 "LED_THT:LED_D3.0mm" V 9675 4650 50  0001 C CNN
F 3 "~" V 9675 4650 50  0001 C CNN
	1    9675 4650
	0    -1   -1   0   
$EndComp
Connection ~ 9675 4750
Wire Wire Line
	9675 4750 9975 4750
$Comp
L 1nofilterbooth:R_Small-Device R_SW_UP?
U 1 1 5E45F215
P 9775 4550
AR Path="/5E3BF423/5E45F215" Ref="R_SW_UP?"  Part="1" 
AR Path="/5E432787/5E45F215" Ref="R_SW_BOTTOM1"  Part="1" 
F 0 "R_SW_BOTTOM1" V 9579 4550 50  0000 C CNN
F 1 "560ohm" V 9670 4550 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" H 9775 4550 50  0001 C CNN
F 3 "~" H 9775 4550 50  0001 C CNN
	1    9775 4550
	0    1    1    0   
$EndComp
$Comp
L 1nofilterbooth:+5V-power #PWR?
U 1 1 5E45F21F
P 9875 4550
AR Path="/5E3BF423/5E45F21F" Ref="#PWR?"  Part="1" 
AR Path="/5E432787/5E45F21F" Ref="#PWR0197"  Part="1" 
F 0 "#PWR0197" H 9875 4400 50  0001 C CNN
F 1 "+5V" V 9890 4678 50  0000 L CNN
F 2 "" H 9875 4550 50  0001 C CNN
F 3 "" H 9875 4550 50  0001 C CNN
	1    9875 4550
	0    1    1    0   
$EndComp
Text GLabel 6225 3375 2    50   Input ~ 0
SPIDER_UPDOWN_PIN_ENDSTOP_UP
Text GLabel 6225 3475 2    50   Input ~ 0
SPIDER_UPDOWN_PIN_ENDSTOP_MIDDLE
Text GLabel 6225 3575 2    50   Input ~ 0
SPIDER_UPDOWN_PIN_ENDSTOP_BOTTOM
$Comp
L 1nofilterbooth:Conn_01x03-Connector_Generic CONN_SW_ROT?
U 1 1 5E46F745
P 5750 6825
AR Path="/5E3BF423/5E46F745" Ref="CONN_SW_ROT?"  Part="1" 
AR Path="/5E432787/5E46F745" Ref="CONN_TEMP1"  Part="1" 
F 0 "CONN_TEMP1" H 5830 6867 50  0000 L CNN
F 1 "JST_XH3" H 5830 6776 50  0000 L CNN
F 2 "Connector_JST:JST_XH_S3B-XH-A-1_1x03_P2.50mm_Horizontal" H 5750 6825 50  0001 C CNN
F 3 "~" H 5750 6825 50  0001 C CNN
	1    5750 6825
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0198
U 1 1 5E471A2B
P 5550 6725
F 0 "#PWR0198" H 5550 6475 50  0001 C CNN
F 1 "GND" V 5555 6597 50  0000 R CNN
F 2 "" H 5550 6725 50  0001 C CNN
F 3 "" H 5550 6725 50  0001 C CNN
	1    5550 6725
	0    1    1    0   
$EndComp
$Comp
L power:+5V #PWR0199
U 1 1 5E47266B
P 5550 6925
F 0 "#PWR0199" H 5550 6775 50  0001 C CNN
F 1 "+5V" V 5565 7053 50  0000 L CNN
F 2 "" H 5550 6925 50  0001 C CNN
F 3 "" H 5550 6925 50  0001 C CNN
	1    5550 6925
	0    -1   -1   0   
$EndComp
Text GLabel 5550 6825 0    50   Input ~ 0
TEMP_PIN
Text GLabel 6225 3675 2    50   Input ~ 0
TEMP_PIN
$Comp
L 1nofilterbooth:Conn_01x03-Connector_Generic CONN_TEMP?
U 1 1 5E474C76
P 5750 7475
AR Path="/5E3BF423/5E474C76" Ref="CONN_TEMP?"  Part="1" 
AR Path="/5E432787/5E474C76" Ref="CONN_LGHT_PROCESS1"  Part="1" 
F 0 "CONN_LGHT_PROCESS1" H 5830 7517 50  0000 L CNN
F 1 "JST_XH3" H 5830 7426 50  0000 L CNN
F 2 "Connector_JST:JST_XH_S3B-XH-A-1_1x03_P2.50mm_Horizontal" H 5750 7475 50  0001 C CNN
F 3 "~" H 5750 7475 50  0001 C CNN
	1    5750 7475
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0200
U 1 1 5E474C80
P 4400 7325
F 0 "#PWR0200" H 4400 7075 50  0001 C CNN
F 1 "GND" V 4405 7197 50  0000 R CNN
F 2 "" H 4400 7325 50  0001 C CNN
F 3 "" H 4400 7325 50  0001 C CNN
	1    4400 7325
	0    1    1    0   
$EndComp
$Comp
L power:+5V #PWR0201
U 1 1 5E474C8A
P 4425 7625
F 0 "#PWR0201" H 4425 7475 50  0001 C CNN
F 1 "+5V" V 4440 7753 50  0000 L CNN
F 2 "" H 4425 7625 50  0001 C CNN
F 3 "" H 4425 7625 50  0001 C CNN
	1    4425 7625
	0    -1   -1   0   
$EndComp
Text GLabel 5550 7475 0    50   Input ~ 0
LIGHT_PROCESS_PIN
Wire Wire Line
	2475 4550 2625 4550
Wire Wire Line
	2125 4550 2475 4550
Connection ~ 2475 4550
$Comp
L 1nofilterbooth:CP-Device C?
U 1 1 5E43ED7D
P 2475 4700
AR Path="/5E3BF423/5E43ED7D" Ref="C?"  Part="1" 
AR Path="/5E432787/5E43ED7D" Ref="C_STEP1"  Part="1" 
F 0 "C_STEP1" H 2593 4746 50  0000 L CNN
F 1 "100uF" H 2593 4655 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D6.3mm_P2.50mm" H 2513 4550 50  0001 C CNN
F 3 "~" H 2475 4700 50  0001 C CNN
	1    2475 4700
	1    0    0    -1  
$EndComp
$Comp
L 1nofilterbooth:CP-Device C?
U 1 1 5E480053
P 4625 7475
AR Path="/5E3BF423/5E480053" Ref="C?"  Part="1" 
AR Path="/5E432787/5E480053" Ref="C_LIGHT_PROCESS1"  Part="1" 
F 0 "C_LIGHT_PROCESS1" H 4743 7521 50  0000 L CNN
F 1 "100uF" H 4743 7430 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D6.3mm_P2.50mm" H 4663 7325 50  0001 C CNN
F 3 "~" H 4625 7475 50  0001 C CNN
	1    4625 7475
	-1   0    0    1   
$EndComp
Wire Wire Line
	4400 7325 4625 7325
Wire Wire Line
	5550 7325 5550 7375
Connection ~ 4625 7325
Wire Wire Line
	4625 7325 5550 7325
Wire Wire Line
	4425 7625 4625 7625
Wire Wire Line
	5550 7625 5550 7575
Connection ~ 4625 7625
Wire Wire Line
	4625 7625 5550 7625
Text GLabel 5550 6300 0    50   Input ~ 0
SERVO_ARM
$Comp
L 1nofilterbooth:Conn_01x03-Connector_Generic CONN_TEMP?
U 1 1 5E4869F5
P 5750 6300
AR Path="/5E3BF423/5E4869F5" Ref="CONN_TEMP?"  Part="1" 
AR Path="/5E432787/5E4869F5" Ref="CONN_SERVO_ARM1"  Part="1" 
F 0 "CONN_SERVO_ARM1" H 5830 6342 50  0000 L CNN
F 1 "JST_XH3" H 5830 6251 50  0000 L CNN
F 2 "Connector_JST:JST_XH_S3B-XH-A-1_1x03_P2.50mm_Horizontal" H 5750 6300 50  0001 C CNN
F 3 "~" H 5750 6300 50  0001 C CNN
	1    5750 6300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0202
U 1 1 5E48D275
P 4400 6150
F 0 "#PWR0202" H 4400 5900 50  0001 C CNN
F 1 "GND" V 4405 6022 50  0000 R CNN
F 2 "" H 4400 6150 50  0001 C CNN
F 3 "" H 4400 6150 50  0001 C CNN
	1    4400 6150
	0    1    1    0   
$EndComp
$Comp
L power:+5V #PWR0203
U 1 1 5E48D27B
P 4425 6450
F 0 "#PWR0203" H 4425 6300 50  0001 C CNN
F 1 "+5V" V 4440 6578 50  0000 L CNN
F 2 "" H 4425 6450 50  0001 C CNN
F 3 "" H 4425 6450 50  0001 C CNN
	1    4425 6450
	0    -1   -1   0   
$EndComp
$Comp
L 1nofilterbooth:CP-Device C_LGHT_PROCESS?
U 1 1 5E48D282
P 4900 6300
AR Path="/5E3BF423/5E48D282" Ref="C_LGHT_PROCESS?"  Part="1" 
AR Path="/5E432787/5E48D282" Ref="C_SERVO_ARM1"  Part="1" 
F 0 "C_SERVO_ARM1" H 5018 6346 50  0000 L CNN
F 1 "100uF" H 5018 6255 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D6.3mm_P2.50mm" H 4938 6150 50  0001 C CNN
F 3 "~" H 4900 6300 50  0001 C CNN
	1    4900 6300
	-1   0    0    1   
$EndComp
Wire Wire Line
	4400 6150 4900 6150
Wire Wire Line
	5550 6150 5550 6200
Connection ~ 4900 6150
Wire Wire Line
	4900 6150 5550 6150
Wire Wire Line
	4425 6450 4900 6450
Wire Wire Line
	5550 6450 5550 6400
Connection ~ 4900 6450
Wire Wire Line
	4900 6450 5550 6450
NoConn ~ 5225 2175
NoConn ~ 5225 2275
NoConn ~ 5225 2375
NoConn ~ 5225 2475
NoConn ~ 5225 5575
NoConn ~ 5225 5675
NoConn ~ 6225 5575
NoConn ~ 6225 5675
NoConn ~ 6225 5775
$Comp
L 1nofilterbooth:C-Device C_MEGA_PAPER1
U 1 1 5E4D5854
P 5075 1525
F 0 "C_MEGA_PAPER1" H 4725 1575 50  0000 L CNN
F 1 "100nF" H 4725 1500 50  0000 L CNN
F 2 "Capacitor_THT:C_Rect_L7.2mm_W2.5mm_P5.00mm_FKS2_FKP2_MKS2_MKP2" H 5113 1375 50  0001 C CNN
F 3 "~" H 5075 1525 50  0001 C CNN
	1    5075 1525
	1    0    0    -1  
$EndComp
Wire Wire Line
	5225 1575 5225 1675
Wire Wire Line
	5225 1475 5225 1375
Wire Wire Line
	5225 1375 5075 1375
Connection ~ 5225 1375
Connection ~ 5075 1375
Wire Wire Line
	5075 1375 4700 1375
Wire Wire Line
	5225 1675 5075 1675
Connection ~ 5225 1675
Connection ~ 5075 1675
Wire Wire Line
	5075 1675 4700 1675
$Comp
L power:+12V #PWR0204
U 1 1 5E4DA24E
P 4700 1375
F 0 "#PWR0204" H 4700 1225 50  0001 C CNN
F 1 "+12V" V 4715 1503 50  0000 L CNN
F 2 "" H 4700 1375 50  0001 C CNN
F 3 "" H 4700 1375 50  0001 C CNN
	1    4700 1375
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR0205
U 1 1 5E4DAE5D
P 4700 1675
F 0 "#PWR0205" H 4700 1425 50  0001 C CNN
F 1 "GND" V 4705 1547 50  0000 R CNN
F 2 "" H 4700 1675 50  0001 C CNN
F 3 "" H 4700 1675 50  0001 C CNN
	1    4700 1675
	0    1    1    0   
$EndComp
Text GLabel 6225 4475 2    50   Input ~ 0
SERVO_ARM
Text GLabel 6225 4375 2    50   Input ~ 0
LIGHT_PROCESS_PIN
Text GLabel 6225 4575 2    50   Input ~ 0
TEMP_PIN
$Comp
L 1nofilterbooth:Conn_01x02-Connector_Generic CONN_SW_MIDDLE?
U 1 1 5E5BAF8E
P 1300 6100
AR Path="/5E5BAF8E" Ref="CONN_SW_MIDDLE?"  Part="1" 
AR Path="/5E3BF423/5E5BAF8E" Ref="CONN_SW_MIDDLE?"  Part="1" 
AR Path="/5E432787/5E5BAF8E" Ref="CONN_DC1"  Part="1" 
F 0 "CONN_DC1" H 1380 6092 50  0000 L CNN
F 1 "JST_XH2" H 1380 6001 50  0000 L CNN
F 2 "Connector_JST:JST_XH_S2B-XH-A-1_1x02_P2.50mm_Horizontal" H 1300 6100 50  0001 C CNN
F 3 "~" H 1300 6100 50  0001 C CNN
	1    1300 6100
	-1   0    0    -1  
$EndComp
Wire Wire Line
	1500 6100 1825 6100
Wire Wire Line
	1500 6200 1825 6200
NoConn ~ 6225 1575
NoConn ~ 6225 1675
NoConn ~ 6225 1775
NoConn ~ 6225 1875
NoConn ~ 6225 2175
NoConn ~ 6225 2275
NoConn ~ 6225 2375
NoConn ~ 6225 2475
NoConn ~ 6225 2575
NoConn ~ 6225 2675
NoConn ~ 6225 2775
NoConn ~ 6225 2875
NoConn ~ 6225 2975
NoConn ~ 6225 3075
NoConn ~ 6225 4975
NoConn ~ 6225 5075
NoConn ~ 6225 5175
NoConn ~ 6225 5275
NoConn ~ 5225 5475
NoConn ~ 5225 5375
NoConn ~ 5225 5275
NoConn ~ 5225 5175
NoConn ~ 5225 5075
NoConn ~ 5225 4975
NoConn ~ 5225 4875
NoConn ~ 5225 4775
NoConn ~ 5225 4675
NoConn ~ 5225 4575
NoConn ~ 5225 4475
NoConn ~ 5225 4375
NoConn ~ 5225 4275
NoConn ~ 5225 4075
NoConn ~ 5225 3975
NoConn ~ 5225 4175
NoConn ~ 5225 2575
$Comp
L power:GND #PWR0209
U 1 1 5E497F5C
P 5175 5800
F 0 "#PWR0209" H 5175 5550 50  0001 C CNN
F 1 "GND" H 5180 5627 50  0000 C CNN
F 2 "" H 5175 5800 50  0001 C CNN
F 3 "" H 5175 5800 50  0001 C CNN
	1    5175 5800
	1    0    0    -1  
$EndComp
Wire Wire Line
	5175 5800 5175 5775
Wire Wire Line
	5175 5775 5225 5775
$Comp
L 1nofilterbooth:TIP120-Transistor_BJT Q_AUX?
U 1 1 5E49604F
P 10475 5825
AR Path="/5E3BF423/5E49604F" Ref="Q_AUX?"  Part="1" 
AR Path="/5E432787/5E49604F" Ref="Q_AUX2"  Part="1" 
F 0 "Q_AUX2" H 10682 5871 50  0000 L CNN
F 1 "TIP120" H 10682 5780 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 10675 5750 50  0001 L CIN
F 3 "http://www.fairchildsemi.com/ds/TI/TIP120.pdf" H 10475 5825 50  0001 L CNN
	1    10475 5825
	1    0    0    -1  
$EndComp
$Comp
L 1nofilterbooth:R_Small-Device R_FLASH?
U 1 1 5E496055
P 10175 5825
AR Path="/5E3BF423/5E496055" Ref="R_FLASH?"  Part="1" 
AR Path="/5E432787/5E496055" Ref="R_FLASH4"  Part="1" 
F 0 "R_FLASH4" V 9979 5825 50  0000 C CNN
F 1 "2.2k" V 10070 5825 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" H 10175 5825 50  0001 C CNN
F 3 "~" H 10175 5825 50  0001 C CNN
	1    10175 5825
	0    1    1    0   
$EndComp
$Comp
L 1nofilterbooth:CP-Device C_AUX?
U 1 1 5E49605B
P 9650 5550
AR Path="/5E3BF423/5E49605B" Ref="C_AUX?"  Part="1" 
AR Path="/5E432787/5E49605B" Ref="C_AUX3"  Part="1" 
F 0 "C_AUX3" H 9768 5596 50  0000 L CNN
F 1 "680uF" H 9768 5505 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D8.0mm_P3.50mm" H 9688 5400 50  0001 C CNN
F 3 "~" H 9650 5550 50  0001 C CNN
	1    9650 5550
	-1   0    0    1   
$EndComp
$Comp
L 1nofilterbooth:GND-power #PWR?
U 1 1 5E496061
P 10575 6025
AR Path="/5E3BF423/5E496061" Ref="#PWR?"  Part="1" 
AR Path="/5E432787/5E496061" Ref="#PWR0214"  Part="1" 
F 0 "#PWR0214" H 10575 5775 50  0001 C CNN
F 1 "GND" H 10580 5852 50  0000 C CNN
F 2 "" H 10575 6025 50  0001 C CNN
F 3 "" H 10575 6025 50  0001 C CNN
	1    10575 6025
	1    0    0    -1  
$EndComp
$Comp
L 1nofilterbooth:+12V-power #PWR?
U 1 1 5E49606C
P 9650 5350
AR Path="/5E3BF423/5E49606C" Ref="#PWR?"  Part="1" 
AR Path="/5E432787/5E49606C" Ref="#PWR0215"  Part="1" 
F 0 "#PWR0215" H 9650 5200 50  0001 C CNN
F 1 "+12V" V 9665 5478 50  0000 L CNN
F 2 "" H 9650 5350 50  0001 C CNN
F 3 "" H 9650 5350 50  0001 C CNN
	1    9650 5350
	1    0    0    -1  
$EndComp
Text Label 10075 5825 2    50   ~ 0
FAN_PIN
$Comp
L 1nofilterbooth:Conn_01x02-Connector_Generic CONN_AUX?
U 1 1 5E496049
P 10975 5350
AR Path="/5E496049" Ref="CONN_AUX?"  Part="1" 
AR Path="/5E3BF423/5E496049" Ref="CONN_AUX?"  Part="1" 
AR Path="/5E432787/5E496049" Ref="CONN_FAN1"  Part="1" 
F 0 "CONN_FAN1" V 10939 5162 50  0000 R CNN
F 1 "JST_XH2" V 10848 5162 50  0000 R CNN
F 2 "Connector_JST:JST_XH_S2B-XH-A-1_1x02_P2.50mm_Horizontal" H 10975 5350 50  0001 C CNN
F 3 "~" H 10975 5350 50  0001 C CNN
	1    10975 5350
	1    0    0    -1  
$EndComp
Wire Wire Line
	10775 5350 9650 5350
Wire Wire Line
	9650 5400 9650 5350
Connection ~ 9650 5350
$Comp
L 1nofilterbooth:GND-power #PWR?
U 1 1 5E4A88C2
P 9650 5750
AR Path="/5E3BF423/5E4A88C2" Ref="#PWR?"  Part="1" 
AR Path="/5E432787/5E4A88C2" Ref="#PWR0216"  Part="1" 
F 0 "#PWR0216" H 9650 5500 50  0001 C CNN
F 1 "GND" H 9655 5577 50  0000 C CNN
F 2 "" H 9650 5750 50  0001 C CNN
F 3 "" H 9650 5750 50  0001 C CNN
	1    9650 5750
	1    0    0    -1  
$EndComp
Wire Wire Line
	9650 5750 9650 5700
Wire Wire Line
	10575 5625 10575 5450
Wire Wire Line
	10575 5450 10775 5450
Text Label 6325 3875 0    50   ~ 0
FAN_PIN
Wire Wire Line
	6325 3875 6225 3875
NoConn ~ 5225 1975
NoConn ~ 5225 2075
NoConn ~ 5225 1875
NoConn ~ 5225 1775
$Comp
L Connector_Generic:Conn_01x10 J_AUX_PAPER1
U 1 1 5E4C98B4
P 1475 6775
F 0 "J_AUX_PAPER1" V 1692 6721 50  0000 C CNN
F 1 "Conn_01x10" V 1601 6721 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x10_P2.54mm_Vertical" H 1475 6775 50  0001 C CNN
F 3 "~" H 1475 6775 50  0001 C CNN
	1    1475 6775
	0    -1   -1   0   
$EndComp
Text Label 1075 6975 3    50   ~ 0
A4
Text Label 1175 6975 3    50   ~ 0
A5
Text Label 6225 1975 0    50   ~ 0
A4
Text Label 6225 2075 0    50   ~ 0
A5
Text Label 6225 4675 0    50   ~ 0
D47
Text Label 6225 3775 0    50   ~ 0
D38
Text Label 6225 3975 0    50   ~ 0
D40
Text Label 6225 4075 0    50   ~ 0
D41
Text Label 6225 4175 0    50   ~ 0
D42
Text Label 6225 4275 0    50   ~ 0
D43
Text Label 6225 4775 0    50   ~ 0
D48
Text Label 6225 4875 0    50   ~ 0
D49
Text Label 1275 6975 3    50   ~ 0
D38
Text Label 1375 6975 3    50   ~ 0
D40
Text Label 1475 6975 3    50   ~ 0
D41
Text Label 1575 6975 3    50   ~ 0
D42
Text Label 1675 6975 3    50   ~ 0
D43
Text Label 1775 6975 3    50   ~ 0
D47
Text Label 1875 6975 3    50   ~ 0
D48
Text Label 1975 6975 3    50   ~ 0
D49
$EndSCHEMATC