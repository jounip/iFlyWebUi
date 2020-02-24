
-- Operates small SimC text display

time1 = 0
timeclear1 = 0

function text(mtype, colour, scroll, delay, id, n, msgs)
	if (n == 0) or ((n == 1) and (string.byte(msgs[1]) == 0)) then 
		file = io.open("gsxtext.txt", "w+")
		io.output(file)
		io.close(file)
	else
		file = io.open("gsxtext.txt", "w+")
		io.output(file)
		io.write(msgs[1])
		io.write("\n")
		io.close(file)
		if delay > 0.9 then
			timeclear1 = delay * 1000
			time1 = ipc.elapsedtime()
		end
	end
end

function time()
    val = ipc.elapsedtime()
	if time1 ~= 0 and ((val - time1) > timeclear1) then
		file = io.open("gsxtext.txt", "w+")
		io.output(file)
		io.close(file)
		time1 = 0
	end
end	

-- Operates SimC Menu display

function menu(mtype, colour, scroll, delay, id, n, msgs)
	if (n == 0) or ((n == 1) and (string.byte(msgs[1]) == 0)) then 
		file = io.open("gsxmenu.txt", "w+")
		io.output(file)
		io.close(file)
	else
		file = io.open("gsxmenu.txt", "w+")
		io.output(file)
		io.write(msgs[1])
		if (n > 1) then
			io.write(" - ")
			io.write(msgs[2])
		end
		io.write("\n")
		i = 3
		j = 1
		while i <= n do
			io.write(j .. " - " .. msgs[i])
			io.write("\n")
			i = i + 1
			j = j + 1
			if (j > 9) then 
				j = 0
			end
		end
	end
end

-- Operates SimC Message Window (lowest priority)

function wind(mtype, colour, scroll, delay, id, n, msgs)
	if (n == 0) or ((n == 1) and (string.byte(msgs[1]) == 0)) then 
		file = io.open("windtext.txt", "w+")
		io.output(file)
		io.close(file)
	else
		file = io.open("windtext.txt", "w+")
		io.output(file)
		io.write(msgs[1])
		if (n > 1) then
			io.write(msgs[2])
		end
	end
end

-- Operates ActiveSky plan weather summary display

function wthr(mtype, colour, scroll, delay, id, n, msgs)
	if (n == 0) or ((n == 1) and (string.byte(msgs[1]) == 0)) then 
		file = io.open("activeskytext.txt", "w+")
		io.output(file)
		io.close(file)
	else
		file = io.open("activeskytext.txt", "w+")
		io.output(file)
		io.write("\n")
		if (n > 1) then
			io.write(msgs[2])
			io.write("\n")
		end
		i = 3
		j = 1
		while i <= n do
			io.write(j .. " - " .. msgs[i])
			io.write("\n")
			i = i + 1
			j = j + 1
			if (j > 9) then 
				j = 0
			end
		end
		io.close(file)
	end
end

ipc.sleep(2000)

event.textmenu(1, "text") -- 1 = SimC text
event.textmenu(2, "menu") -- 2 = SimC menu
event.textmenu(8, "wthr") -- 2 = ASN Weather
event.textmenu(4, "wind") -- 4 = SimC message wind

event.timer(500, "time")
