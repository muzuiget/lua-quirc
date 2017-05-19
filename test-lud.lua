local quirc = require('quirc')
local readraw = require('readraw')

local print_result = function(filename, result)
    print(filename)
    if result then
        for k, v in pairs(result) do
            print(string.format('  %s: %s', k, v))
        end
    else
        print('  nil')
    end
end

local test = function(filename, width, height)
    width = tonumber(width)
    height = tonumber(height)
    local lud = readraw(filename, width, height)
    if not lud then
        print('Error: not a userdata')
        return
    end
    local result = quirc.scan_lud(lud)
    print_result(filename, result)
end

if #arg ~= 2 then
    print(string.format('Usage:  lua test-raw.lua image.raw width:height'))
    return
end

local width, height = string.match(arg[2], '(%d+):(%d+)')
test(arg[1], width, height)
