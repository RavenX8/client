
fn is_poly obj =
(
    if (((classOf obj) == Editable_Poly) or ((classOf obj) == PolyMeshObject)) do return true
    return false
)

fn is_mesh obj =
(
    if ((classOf obj) == Editable_Mesh) do return true
    return false
)

fn is_mesh_or_poly obj =
(
    if ((is_mesh obj) or (is_poly obj)) do return true
    return false
)

fn get_num_mapchannel obj =
(
    if (is_poly obj) then
    (
        return polyop.getNumMaps obj
    )
    if (is_mesh obj) then
    (
        return meshop.getNumMaps obj
    )
    messageBox (obj.name + "이 Mesh 혹은 Poly 가 아닙니다.")
    
    return undefined
)

fn get_map_support obj channel =
(
    map_support = false
    if (is_poly obj) then
    (
        map_support = polyop.getMapSupport obj channel
    )
    else if (is_mesh obj) then
    (
        map_support = meshop.getMapSupport obj channel
    )
    else
    (
        messageBox (obj.name + "이 Mesh 혹은 Poly 가 아닙니다.")
        return undefined
    )
    print ("map_support" + (channel as string) + " = " + (map_support as string))
    return map_support
)
 
fn set_map_support obj channel true_or_false =
(
    if (is_poly obj) then
    (
        if ((polyop.getNumMaps obj) < channel) then
        (
            polyop.setNumMaps obj channel
        )
        if ((polyop.getMapSupport obj channel) != true_or_false) then
        (
            polyop.setMapSupport obj channel true_or_false
         )
         return true
    )
    if (is_mesh obj) then
    (
        if ((meshop.getNumMaps obj) < channel) then
        (
            meshop.setNumMaps obj channel
        )
        if ((meshop.getMapSupport obj channel) != true_or_false) then
        (
            meshop.setMapSupport obj channel true_or_false
        )
        return true
    )
    messageBox (obj.name + "이 Mesh 혹은 Poly 가 아닙니다.")
    
    return undefined
 )
