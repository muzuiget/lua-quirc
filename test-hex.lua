local quirc = require('quirc')

local read_file = function(filename)
    local file = io.open(filename, 'rb')
    local data = file:read('*all')
    file:close()
    local width = (data:find('\n') - 1) / 2
    local height = width
    local image = data:gsub('\n', ''):gsub('00', '\x00'):gsub('ff', '\xFF')
    return image, width, height
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

local test = function(filename)
    local image, width, height = read_file(filename)
    assert(#image == (width * height));
    local result = quirc.scan(image, width, height)
    print_result(filename, result)
end

if #arg ~= 1 then
    print(string.format('Usage: lua test-hex.lua'))
    return
end

test(arg[1])
