describe 'database' do
    def run_script(commands)
      raw_output = nil
      IO.popen("./db", "r+") do |pipe|
        commands.each do |command|
          pipe.puts command
        end
  
        pipe.close_write

        raw_output = pipe.gets(nil)
      end
      raw_output.split("\n")
    end
  
    it 'test exit and unrecognized command' do
      result = run_script([
        "hello world",
        "HELLO WORLD",
        ".exit",
      ])
      expect(result).to match_array([
        "database > Unrecognized command: hello world",
        "database > Unrecognized command: HELLO WORLD",
        "darabase > Bye!",
      ])
    end
end
 