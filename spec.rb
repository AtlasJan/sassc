#!/usr/bin/env ruby

searchpath = ARGV[0]

messages = []
warnings = []
worked = 0
spec_count = 0

puts("Running tests in #{searchpath}..\n\n")

Dir["**/input.*"].each do |input_file|
  if input_file[0..(searchpath.length - 1)] == (searchpath)
    spec_count += 1
    spec_dir = File.dirname(input_file)
    cmd = "./bin/sassc #{input_file}"
    #puts cmd
    output = `#{cmd}`
    sass_output = `sass #{input_file}`
    expected_output = File.read(File.join(spec_dir, "output.css"))
    if expected_output.strip != sass_output.strip 
      warnings << "Problem with Ruby compat in #{input_file}"
    end
    if output.strip != expected_output.strip
      print "F"
      messages << "Failed test #{spec_dir}"
    else
      worked += 1
      print "."
    end
  end
end

puts("\n\n#{worked}/#{spec_count} Specs Passed!")

if warnings.length > 0 
  warn = ([""] + warnings).join("\n-----WARN------\n")
  puts("\n#{warn}")
end

if messages.length > 0 
  puts("\n================================\nTEST FAILURES!\n\n")
  puts(messages.join("\n-----------\n"))
  puts("\n")
  exit(1)
else
  puts("YOUWIN!")
  exit(0)
end

