function textandmenu(mtype, colour, scroll, delay, id, n, msgs)
	if n > 0 then
	-- ipc.log("text and menu function type ".. mtype .. " messages ".. n)
		if mtype == 2 then
			-- Menu
			file = io.open("gsxmenu.txt", "w+")
			io.output(file)
			--io.write(msgs[1])
			--io.write("\n")
			--io.write(msgs[2])
			--io.write("\n")
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
		else
		-- Text
			file = io.open("gsxtext.txt", "w+")
			io.output(file)
			io.write(msgs[1])
			io.write("\n")
			io.close(file)
		end
	end
end

event.textmenu(0, "textandmenu")
