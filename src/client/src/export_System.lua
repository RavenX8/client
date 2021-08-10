--- 루아용 인터페이스 모듈 자동 생성하는 스크립트
--- 입력파일 : System_FUNC.h
--- 출력파일 : System_FUNC_DEF.inc System_FUNC_REG.inc

--- 파일을 읽어 interfaces 테이블에 정보 저장
function read_interfaces ( hin, hout )
    i = 1
    word = hin:read( "l" )

    while ( word ) do
        if (word ~= "SYSTEM_SCRIPT") then
            --word = hin:read( "l" ) -- skip current line
        else
            local return_type
            local name
            local words = {}
            local line = hin:read( "l" )
            words[1], words[2] = line:match( "(%g+)(.+)" )
            return_type = words[1]
            if (not return_type) then break end

            words[1], words[2] = words[2]:match( "(%g+)(.+)" )
            name = words[1]
            if (not name) then break end

            interfaces[name] = {}
            interfaces[name].return_type = return_type

            words[1], words[2] = words[2]:match( "(%g+)(.+)" ) -- skip (
            if (not words[1]) then break end

            words[1], words[2] = words[2]:match( "(%g+)(.+)" )
            if (not words[1]) then break end

            interfaces[name].params = {}
            local param_index = 1
            
            while (words[1] ~= ")") do
                -- extract out comma(,)
                interfaces[name].params[param_index] = string.gsub( words[1], ",", "" )
                words[1], words[2] = words[2]:match( "(%g+)(.+)" ) -- skip parameter name
                if (not words[1]) then break end
                if (words[1] == ")") then break end
                words[1], words[2] = words[2]:match( "(%g+)(.+)" ) -- read next
                if (not words[1]) then break end
                if (words[1] == ")") then break end
                param_index = param_index + 1
            end
            i = i + 1
        end -- end of if
        word = hin:read( "l" ) -- read next
    end -- end of while
end

IN_FILENAME   = "System/System_FUNC.h"
OUT_FILENAME1 = "System/System_FUNC_DEF.inc"
OUT_FILENAME2 = "System/System_FUNC_REG.inc"

hin   = io.open ( IN_FILENAME   ) -- input file handle
hout1 = io.open  ( OUT_FILENAME1, "w" ) -- output file handle
hout2 = io.open  ( OUT_FILENAME2, "w" ) -- output file handle

interfaces = {} -- 인터페이스 정보 테이블

--- 인터페이스 정보 읽어들이기
read_interfaces ( hin, hout )

names = {} --- 정렬용 이름 테이블
i = 1
index = next(interfaces, nil) -- 이름으로 참조하는 테이블이므로, interfaces[i] 를 사용할 수 없다.
while (index) do
    names[i] = index
    index = next(interfaces, index)
    i = i + 1
end

--- 이름순으로 정렬하기
table.sort( names )

--- 매크로 참조 테이블들
map_return_types = {
    void    = "RETURNS_NONE",
    int     = "RETURNS_INT",
    float   = "RETURNS_FLOAT",
    HNODE   = "RETURNS_HNODE",
    ZSTRING = "RETURNS_STRING"
}
map_param_types = {
    void    = "",
    int     = "ZL_INT",
    float   = "ZL_FLOAT",
    HNODE   = "ZL_HNODE",
    ZSTRING = "ZL_STRING"
}

--- 경고 메세지
message_autogen =
"//==========================================================================//\n"..
"// 이 파일은 export_interface.lua 스크립트에 의해 자동생성된 코드입니다.\n"..
"// 직접 수정하지 마십시오.\n"..
"// "..os.date().."\n"..
"//==========================================================================//\n\n\n"

hout1:write( message_autogen )
hout2:write( message_autogen )

--- 스크립트 인터페이스 정의용 파일 생성하기
for i = 1, #names do
    definition = string.format( "ZL_FUNC_BEGIN( %s, ", names[i] )
    -- export return_type
    definition = definition..string.format( "%s", map_return_types[ interfaces[names[i]].return_type ] )
    definition = definition.." )\n"
    
    for j = 1, #(interfaces[names[i]].params) do
        if (j == #(interfaces[names[i]].params)) then
            definition = definition..string.format( "\t%15s\n", map_param_types[ interfaces[names[i]].params[j] ] )
        else
            definition = definition..string.format( "\t%15s,\n", map_param_types[ interfaces[names[i]].params[j] ] )
        end
    end

    definition = definition.."ZL_FUNC_END\n\n"

    hout1:write( definition )
end

--- 스크립트 인터페이스 등록용 파일 생성하기
for i = 1, #(names) do
    register = string.format( "ZL_REGISTER( %-30s )\n", names[i] )
    hout2:write( register )
end

--- 파일의 끝 표시
message_end_of_file = "// end of file\n"

hout1:write( message_end_of_file )
hout2:write( message_end_of_file )

io.close( hin   )
io.close( hout1 )
io.close( hout2 )

