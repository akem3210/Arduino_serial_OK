
def init()
	puts "init()..."
	puts "init().ok."
end

def handle_input(midi_input = [ "0", 0, 0 ]) # midi_input = [ midi_name(string), midi_ID(int), midi_val(int 0-127) ]
	puts midi_input.inspect
	####### PADS
	#######
	if(midi_input[0].include? "Note on") then
  		# Each pad has a number in here
  		case midi_input[1]
  			when 1;
			  		t = Thread.new(){
	  					system("play -n synth pl C#{1 + rand(4)} pl E2 pl G#{1 + rand(4)} pl C3 pl E#{1 + rand(4)} pl G3 delay 0 .05 .1 .15 .2 .25 remix - fade 0 1.5 .1 norm -1 speed 0.2 > /dev/null 2>&1")
					}
  		end
  	end
	###### FADERS AND KNOBS
	######
	if(midi_input[0].include? "Control") then
  		# Each knob or fader has a number in here
  		case midi_input[1]
			when 16;	t = Thread.new(){
						system("play -n synth pl C#{1 + rand(4)} pl E2 pl G#{1 + rand(4)} pl C3 pl E#{1 + rand(4)} pl G3 delay 0 .05 .1 .15 .2 .25 remix - fade 0 1.5 .1 norm -1 speed 0.2 > /dev/null 2>&1")
					}
		end
	end
end
