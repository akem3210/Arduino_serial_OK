
=begin

16 20 24 28 46 50 54 58 | XX
17 21 25 29 47 51 55 59 | 25
18 22 26 30 48 52 56 60 | 26
-------------------------
01 04 07 10 13 16 19 22   27
03 06 09 12 15 18 21 24
-------------------------
19 23 27 31 49 53 57 61   62

Les knobs et faders vont de 0 à 127.
les boutons sont "Note on" quand activé avec une valeur fixe à 127.

=end


def init()
	puts "init()..."
	puts "init().ok."
end

# ------------------------------------------------------------------------
# Returns linear 0...X from the MIDI assigned ID from AKAI ---------------
$midi_pad_num_ids = [ 1,3,4,6,7,9,10,12,13,15,16,18,19,21,22,24 ]
def get_linear_PAD_ID(midi_input_1) # midi_input[1]
	id = -1
	0.upto($midi_pad_num_ids.size - 1){ |i|
		if(midi_input_1 == $midi_pad_num_ids[i]) then
			id = i;
			break
		end
	}
	return id
end
# ------------------------------------------------------------------------

def handle_input(midi_input = [ "0", 0, 0 ]) # midi_input = [ midi_name(string), midi_ID(int), midi_val(int 0-127) ]
	puts midi_input.inspect

	#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	if(midi_input[0].include? "Note on") then
		# button to change samples bank to the left
		if (midi_input[1] == 26) then
$sp.write("{LED_ON}")
		end
		# button to change samples bank to the right
		if (midi_input[1] == 25) then
$sp.write("{LED_OFF}")
		end
		# line 1 and 2 buttons plays samples - plays samples
	  	if (midi_input[1] < 22) then
			id = get_linear_PAD_ID(midi_input[1])
			$sp.write("{BUTTON=#{midi_input[1]}}")
		end
		# last button before 'solo', line 1 -
		if (midi_input[1] == 22) then
		end
		# last button before 'solo', line 2 - stops all sample plays instantly
		if (midi_input[1] == 24) then
		end
	end
	#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


	####### PADS
	#######
	if(midi_input[0].include? "Note on") then
  		case midi_input[1]
			when 1;
			when 3;
			when 4;
			when 6;
			when 7;
			when 9;
			when 10;
			when 12;
			when 13;
			when 15;
			when 16;
			when 18;
			when 19;
			when 21;
			when 22;
			when 24;
  		end
  	end
  	
  	#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	# last line of knobs affect samples played with buttons
	if(midi_input[0].include? "Control") then
  		case midi_input[1]
			when 18;
			when 22;
			when 26;
			when 30;
			when 48;
			when 52;
			when 56;
			when 60;
			when 49;
			when 53;
			when 57;
			when 61;
			when 62;
		end
	end
  	#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  	
	###### FADERS AND KNOBS
	######
	if(midi_input[0].include? "Control") then
  		case midi_input[1]
  			when 16; if not defined? $last_step_A then
  				 	$last_step_A = 0
  				 end
  				 step = midi_input[2] - $last_step_A
  				 puts "MOVING STEP A #{step}"
#  				 $sp.write("{KNO=16}")
  				 $sp.write("{STEPS_A=#{step}}")
  				 $last_step_A = midi_input[2]

  			when 17; speed = midi_input[2]
  				 puts "MOVING SPEED A #{speed}"
#  				 $sp.write("{KNO=17}")
  				 $sp.write("{SPEED_A=#{speed}}")

  			when 18; $sp.write("{KNOB=18}")
			when 19; $sp.write("{FADER=19}")

  			when 20; if not defined? $last_step_B then
  				 	$last_step_B = 0
  				 end
  				 step = midi_input[2] - $last_step_B
  				 puts "MOVING STEP B #{step}"
#  				 $sp.write("{KNO=16}")
  				 $sp.write("{STEPS_B=#{step}}")
  				 $last_step_B = midi_input[2]

  			when 21; speed = midi_input[2]
  				 puts "MOVING SPEED B #{speed}"
#  				 $sp.write("{KNO=17}")
  				 $sp.write("{SPEED_B=#{speed}}")
  				 

		end
	end
end
