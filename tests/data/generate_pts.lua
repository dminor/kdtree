#!/usr/bin/lua

--[[
Copyright (c) 2010 Daniel Minor 

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
--]]

math.randomseed(os.time())

if #arg ~= 4 then
    print('usage: generate_pts.lua <point count> <dim> <scale> <output>')
    return
end

pt_count = tonumber(arg[1])
dim = tonumber(arg[2])
scale = tonumber(arg[3])
filename = arg[4]

f = assert(io.open(filename, 'w'))
f:write(pt_count .. ' ' .. dim .. '\n')

for i=1,pt_count do
    for i=1, dim do
        f:write(math.random()*scale)
        if (i < dim) then
            f:write(', ')
        end
    end
    f:write('\n')
end

f:close()
