require 'rake'
require 'rake/clean'

NAME = 'bridgerator'

CC = ENV['CC'] || 'clang++'
CFLAGS = "#{ENV['CFLAGS']} -Wall -I ./include  --std=c++11"

SOURCES = FileList['src/*.cpp']
OBJECTS = SOURCES.ext('o')
CLEAN.include(OBJECTS).include(NAME)

task :default => NAME

file NAME => OBJECTS do
	sh "#{CC} #{OBJECTS} -o #{NAME}"
end

rule '.o' => '.cpp' do |file|
	sh "#{CC} #{CFLAGS} -c #{file.source} -o #{file}"
end

