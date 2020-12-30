open System.IO

let Manhattan(x, y, _) = abs(x) + abs(y)

let rec NewDir d (c, deg) =
    let nd = match c with | 'L' -> (d+3)%4 | _ -> (d+1)%4
    match deg > 90 with | true -> NewDir nd (c, deg-90) | _-> nd

let directions = [|(-1,0); (0,1); (1,0); (0,-1)|]

let Part1 (x, y, dir) (d, n) =
    match d with | 'F' -> (x+ fst directions.[dir] * n, y + snd directions.[dir] * n, dir)
                 | 'E' -> (x + fst directions.[0] * n, y, dir)
                 | 'S' -> (x, y + snd directions.[1] * n, dir)
                 | 'W' -> (x + fst directions.[2] * n, y, dir)
                 | 'N' -> (x, y + snd directions.[3] * n, dir)
                 | _ -> (x, y, NewDir dir (d, n))

let rec RotateWaypoint (wx, wy) (c, deg) =
    let nw = match c with | 'L' -> (-wy, wx) | _ -> (wy,-wx)
    match deg > 90 with | true -> RotateWaypoint nw (c, deg-90) | _ -> nw

let Part2 (x, y, (wx, wy)) (d,n) =
    match d with | 'F' -> (x + wx*n, y + wy*n, (wx,wy))
                 | 'E' -> (x, y, (wx + fst directions.[0] * n, wy))
                 | 'S' -> (x, y, (wx, wy + snd directions.[1] * n))
                 | 'W' -> (x, y, (wx + fst directions.[2] * n, wy))
                 | 'N' -> (x, y, (wx, wy + snd directions.[3] * n))
                 | _ -> (x, y, RotateWaypoint (wx, wy) (d,n))

[<EntryPoint>]
let main argv =
    let directions = File.ReadAllLines(argv.[0]) |> Array.map(fun (s:string) -> (s.[0], s.[1..] |> System.Int32.Parse))
    Manhattan (Array.fold(Part1) (0,0,0) directions) |> printfn "Part 1: %A" 
    Manhattan (Array.fold(Part2) (0,0,(-10,-1)) directions) |> printfn "Part 2: %A"
    0 // return an integer exit code
