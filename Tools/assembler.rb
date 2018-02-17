# This is a very basic ABC assembler with absolutely NO program
# verification. You can enter very incorrect programs and they will still
# compile. There is certainly room for improvement!

# These can simply be copied out of the C enum
INSTRUCTIONS = "	NOP    = 0b11111111,
HLT    = 0b11111110,
SLP    = 0b11111101,

MOV    = 0b00000000,
ALCP   = 0b00000001,
DSTP   = 0b00000010,
LOAD   = 0b00000011,
STORE  = 0b00000100,
ADD    = 0b00000101,
SUB    = 0b00000110,
INC    = 0b00000111,
DEC    = 0b00001000,

OUT    = 0b00001001,
OUTN   = 0b00001010,
OUTTC  = 0b00001011,

JMP    = 0b00010000,"

INSTRUCTION_MAP = INSTRUCTIONS.split(",").map { |l|
    item = l.split("=").map {|x| x.strip }
    item[1] = item[1].to_i(2)
    item
}.to_h

filename = ARGV[0]
input = File.read(filename)

result_bytes = []

input.split.each do |x|
    # Integer literals
    if x[0] == "#"
        x[0] = ""
        x = x.to_i

        case x
        when 0..63 then
            result_bytes << x
        when 63..255 then
            result_bytes << 0b01000000
            result_bytes << x
        when 65535 then
            throw "Multi-byte literals are not implemented yet"
        else
            throw "Unable to categorize literal #{x} - this is probably a bug"
        end

    # Register literals
    elsif x[0] == "R"
        x[0] = ""
        result_bytes << (0b11000000 | x.to_i)

    # Instruction
    elsif INSTRUCTION_MAP.keys.include? x 
        result_bytes << INSTRUCTION_MAP[x]
        
    else
        throw "Unknown token #{x}"
    end
end

puts "Length: #{result_bytes.size}"
p result_bytes
puts result_bytes.map { |x| "%02X" % x }.join