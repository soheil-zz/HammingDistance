require 'socket'

server = TCPServer.open(8880)
loop do
  Thread.start(server.accept) do |client|
    begin
      request = client.gets
      raise 'No request' if request.blank?
      request = request.split(',')
      raise 'Incomplete request' if request.size != 3

      phash = request[0].to_i
      threshold = request[1].to_i
      how_many_needed = request[2].to_i

      count = 0
      File.foreach('/tmp/data') do |line|
        # calculate hamming distance
        if (line.to_i ^ phash).to_s(2).count('1') < threshold
          client.puts(line.to_i)
          count += 1
          return if count >= how_many_needed
        end
      end
    rescue
    ensure
      client.close
    end
  end
end