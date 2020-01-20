--LUA script to handle SimConnect menu and text
--
-- This file will integrate menu and text to file that iFlyWebUi can read
-- 


function menu(mtype, colour, scroll, delay, id, n, msgs)
	if (n == 0) or ((n == 1) and (string.byte(msgs[1]) == 0)) then 
		--empty menu
			file = io.open("gsxmenu.txt", "w+")
			io.output(file)
			io.close(file)
	else
		-- Menu not empty
		file = io.open("gsxmenu.txt", "w+")
		io.output(file)
		io.write(msgs[1])
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

function text(mtype, colour, scroll, delay, id, n, msgs)
	if (n == 0) or ((n == 1) and (string.byte(msgs[1]) == 0)) then 
		--empty text
		file = io.open("gsxtext.txt", "w+")
		io.output(file)
		io.close(file)
		prevmsg = ""
	else
		if msgs[1] == prevmsg then
			--empty text
			file = io.open("gsxtext.txt", "w+")
			io.output(file)
			io.close(file)
		end      
		prevmsg = msgs[1]
			-- text to write
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
		--empty text
		file = io.open("gsxtext.txt", "w+")
		io.output(file)
		io.close(file)
		time1 = 0
	end
end	


event.textmenu(2, "menu") -- 2 = SimC menu
event.textmenu(1, "text") -- 1 = SimC text

event.timer(500, "time")
