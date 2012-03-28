#!/usr/bin/env ruby

include Process

DESCRIPTOR = {0 => $stdin, 1 => $stdout, 3 => $stderr}

begin
  begin
    print "%>"
    ligne = gets.downcase.chomp
  rescue NoMethodError => e
    puts "Rentrez une chaine de caractere !"
    retry
  end
  break if ligne == 'exit' 

  begin
    executeAfter ||= ligne
    executeNow, executeAfter = executeAfter.split('|', 2).map{|cmd| cmd.strip} unless executeAfter.nil?
    commande, redirection = executeNow.split(/[<|>]/, 2).map{|c| c.strip}
    p commande
    p redirection
    fork do
      filename = 'tmp'
      File.open(filename, File::CREAT|File::WRONLY|File::TRUNC) do |file|
        res = %x["#{commande}"]
        file.write res
      end
    end
    wait
  end until executeAfter.nil?
rescue Exception => e
  puts e
  exit
end while true
