local quirc = require('quirc')

local read_file = function(filename)
    local file = io.open(filename, 'rb')
    local image = file:read('*all')
    file:close()
    return image
end

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
    local image = read_file(filename)
    local result = quirc.scan(image, width, height)
    print_result(filename, result)
end

if #arg ~= 2 then
    print(string.format('Usage:  lua test-raw.lua image.raw width:height'))
    return
end

local width, height = string.match(arg[2], '(%d+):(%d+)')
test(arg[1], width, height)
