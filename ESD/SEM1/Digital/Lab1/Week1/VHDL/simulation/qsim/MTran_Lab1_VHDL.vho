-- Copyright (C) 2023  Intel Corporation. All rights reserved.
-- Your use of Intel Corporation's design tools, logic functions 
-- and other software and tools, and any partner logic 
-- functions, and any output files from any of the foregoing 
-- (including device programming or simulation files), and any 
-- associated documentation or information are expressly subject 
-- to the terms and conditions of the Intel Program License 
-- Subscription Agreement, the Intel Quartus Prime License Agreement,
-- the Intel FPGA IP License Agreement, or other applicable license
-- agreement, including, without limitation, that your use is for
-- the sole purpose of programming logic devices manufactured by
-- Intel and sold by Intel or its authorized distributors.  Please
-- refer to the applicable agreement for further details, at
-- https://fpgasoftware.intel.com/eula.

-- VENDOR "Altera"
-- PROGRAM "Quartus Prime"
-- VERSION "Version 23.1std.0 Build 991 11/28/2023 SC Lite Edition"

-- DATE "05/10/2024 00:05:09"

-- 
-- Device: Altera 5CSEMA6F31C6 Package FBGA896
-- 

-- 
-- This VHDL file should be used for Questa Intel FPGA (VHDL) only
-- 

LIBRARY ALTERA_LNSIM;
LIBRARY CYCLONEV;
LIBRARY IEEE;
USE ALTERA_LNSIM.ALTERA_LNSIM_COMPONENTS.ALL;
USE CYCLONEV.CYCLONEV_COMPONENTS.ALL;
USE IEEE.STD_LOGIC_1164.ALL;

ENTITY 	and_gate IS
    PORT (
	a : IN std_logic;
	b : IN std_logic;
	output : BUFFER std_logic
	);
END and_gate;

-- Design Ports Information
-- output	=>  Location: PIN_AF28,	 I/O Standard: 2.5 V,	 Current Strength: Default
-- a	=>  Location: PIN_Y23,	 I/O Standard: 2.5 V,	 Current Strength: Default
-- b	=>  Location: PIN_Y24,	 I/O Standard: 2.5 V,	 Current Strength: Default


ARCHITECTURE structure OF and_gate IS
SIGNAL gnd : std_logic := '0';
SIGNAL vcc : std_logic := '1';
SIGNAL unknown : std_logic := 'X';
SIGNAL devoe : std_logic := '1';
SIGNAL devclrn : std_logic := '1';
SIGNAL devpor : std_logic := '1';
SIGNAL ww_devoe : std_logic;
SIGNAL ww_devclrn : std_logic;
SIGNAL ww_devpor : std_logic;
SIGNAL ww_a : std_logic;
SIGNAL ww_b : std_logic;
SIGNAL ww_output : std_logic;
SIGNAL \~QUARTUS_CREATED_GND~I_combout\ : std_logic;
SIGNAL \a~input_o\ : std_logic;
SIGNAL \b~input_o\ : std_logic;
SIGNAL \output~0_combout\ : std_logic;
SIGNAL \ALT_INV_b~input_o\ : std_logic;
SIGNAL \ALT_INV_a~input_o\ : std_logic;

BEGIN

ww_a <= a;
ww_b <= b;
output <= ww_output;
ww_devoe <= devoe;
ww_devclrn <= devclrn;
ww_devpor <= devpor;
\ALT_INV_b~input_o\ <= NOT \b~input_o\;
\ALT_INV_a~input_o\ <= NOT \a~input_o\;

-- Location: IOOBUF_X89_Y13_N56
\output~output\ : cyclonev_io_obuf
-- pragma translate_off
GENERIC MAP (
	bus_hold => "false",
	open_drain_output => "false",
	shift_series_termination_control => "false")
-- pragma translate_on
PORT MAP (
	i => \output~0_combout\,
	devoe => ww_devoe,
	o => ww_output);

-- Location: IOIBUF_X89_Y13_N4
\a~input\ : cyclonev_io_ibuf
-- pragma translate_off
GENERIC MAP (
	bus_hold => "false",
	simulate_z_as => "z")
-- pragma translate_on
PORT MAP (
	i => ww_a,
	o => \a~input_o\);

-- Location: IOIBUF_X89_Y13_N21
\b~input\ : cyclonev_io_ibuf
-- pragma translate_off
GENERIC MAP (
	bus_hold => "false",
	simulate_z_as => "z")
-- pragma translate_on
PORT MAP (
	i => ww_b,
	o => \b~input_o\);

-- Location: LABCELL_X88_Y13_N0
\output~0\ : cyclonev_lcell_comb
-- Equation(s):
-- \output~0_combout\ = ( \b~input_o\ & ( \a~input_o\ ) )

-- pragma translate_off
GENERIC MAP (
	extended_lut => "off",
	lut_mask => "0000000000000000000000000000000000001111000011110000111100001111",
	shared_arith => "off")
-- pragma translate_on
PORT MAP (
	datac => \ALT_INV_a~input_o\,
	dataf => \ALT_INV_b~input_o\,
	combout => \output~0_combout\);

-- Location: LABCELL_X13_Y28_N3
\~QUARTUS_CREATED_GND~I\ : cyclonev_lcell_comb
-- Equation(s):

-- pragma translate_off
GENERIC MAP (
	extended_lut => "off",
	lut_mask => "0000000000000000000000000000000000000000000000000000000000000000",
	shared_arith => "off")
-- pragma translate_on
;
END structure;


