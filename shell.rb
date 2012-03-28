#!/usr/bin/env ruby

include Process

begin
  print "%>"
  break if (ligne = gets.downcase.chomp) == 'exit'
  executeNow, executeAfter = ligne.split('|').map{|cmd| cmd.strip}

  begin
    commande, redirection = executeNow.split(%r{<|>}).map{|c| c.strip}
    fork do
      filename = 'tmp'
      File.open(filename, File::CREAT|File::WRONLY) do |file|
        res = %x["#{commande}"]
        file.write res
      end
    end
    wait
    executeNow, executeAfter = executeAfter.split('|').map{|cmd| cmd.strip} unless executeAfter.nil?
  end until executeAfter.nil?
end while true

