open System.IO

let GetCubes cubes (i, s:string) =
    s.ToCharArray() |> Array.indexed |> Array.fold(fun x (y, c) -> if c = '#' then Array.append x [|(i, y)|] else x) cubes

let GetNew (next, old) (x, i) =
    if i = 3 || (i = 2 && Array.tryFind(fun e -> x = e) old <> None) then (Array.append next [|x|], old)
    else (next, old)

let rec Conway cubes left Expand =
    if left = 0 then Array.length cubes
    else let work = Array.fold Expand [||] cubes |> Array.countBy(id)
         let (next,_) = Array.fold GetNew ([||], cubes) work
         Conway next (left-1) Expand

let Expand3D e (x,y,z) = 
    let pos = seq { for nx in x-1 .. x+1 do
                        for ny in y-1 .. y+1 do
                            for nz in z-1 .. z+1 do (nx, ny, nz)} |> Seq.filter(fun (nx, ny, nz) -> nx <> x || ny <> y || nz <> z)
    Array.append e (Seq.toArray pos)

let Expand4D e (x, y, z, w) =
    let pos = seq { for nx in x-1 .. x+1 do
                        for ny in y-1 .. y+1 do
                            for nz in z-1 .. z+1 do 
                                for nw in w-1 .. w+1 do (nx, ny, nz, nw)} |> Seq.filter(fun (nx, ny, nz, nw) -> nx <> x || ny <> y || nz <> z || nw <> w)
    Array.append e (Seq.toArray pos)

[<EntryPoint>]
let main argv =
    let cubes = File.ReadAllLines(argv.[0]) |> Array.indexed |> Array.fold GetCubes [||]
    Conway (Array.map(fun (x,y) -> (x,y,0)) cubes) 6 Expand3D |> printfn "Part 1: %A"
    Conway (Array.map(fun (x,y) -> (x,y,0,0)) cubes) 6 Expand4D |> printfn "Part 2: %A"
    0
