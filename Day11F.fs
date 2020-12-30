open System.IO

let OccupiedCount (s:string) =
    s.ToCharArray() |> Array.fold(fun total c -> total + match c with | '#' -> 1 | _-> 0) 0

let CountOccupied x y (grid:(int*string)[]) =
    let tmp1 = grid.[(max 0 (y-1)) .. (min (y+1) (grid.Length-1))]
    let area = tmp1 |>  Array.map(fun (_, s:string) -> s.[(max 0 (x-1)) .. (min (x+1) (s.Length-1))])
    let neighbors = Array.fold(fun total (s:string) -> total + OccupiedCount s) 0 area
    neighbors - match snd(grid.[y]).[x] = '#' with | true -> 1 | _-> 0

let rec Simulate1 (grid:(int*string)[]) =
    let FoldRow (y, s:string) =
        s.ToCharArray() |> Array.fold(fun (newString:string) c -> 
            newString + (match c with | '.' -> "."
                                      | 'L' -> match CountOccupied newString.Length y grid with | 0 -> "#" | _-> "L"
                                      | _ -> match (CountOccupied newString.Length y grid) >= 4 with | true -> "L" | _-> "#")) "" 

    let newState = Array.map(FoldRow) grid |> Array.indexed
    match newState = grid with | true -> Array.fold(fun total (_, s) -> total + OccupiedCount s) 0 grid
                               | false -> Simulate1 newState

let rec VisCheck (dx, dy) (x, y) (grid:(int*string)[]) =
    match x < 0 || y < 0 || y >= grid.Length || x >= (snd grid.[0]).Length with 
        | true -> 0
        | _ -> match snd(grid.[y]).[x] with | 'L' -> 0
                                            | '#' -> 1
                                            | _ -> VisCheck (dx,dy) (x+dx, y+dy) grid

let directions = [|(-1,-1); (-1,0); (-1,1); (0,1); (1,1); (1,0); (1,-1); (0,-1)|]

let CountVisible x y (grid:(int*string)[]) =
    Array.fold(fun total (dx, dy) -> total + VisCheck (dx, dy) (x+dx, y+dy) grid) 0 directions

let rec Simulate2 (grid:(int*string)[]) =
    let FoldRow (y, s:string) =
        s.ToCharArray() |> Array.fold(fun (newString:string) c -> 
            newString + (match c with | '.' -> "."
                                      | 'L' -> match CountVisible newString.Length y grid with | 0 -> "#" | _-> "L"
                                      | _ -> match (CountVisible newString.Length y grid) >= 5 with | true -> "L" | _-> "#")) ""
                                      
    let newState = Array.map(FoldRow) grid |> Array.indexed
    match newState = grid with | true -> Array.fold(fun total (_, s) -> total + OccupiedCount s) 0 grid
                               | false -> Simulate2 newState

[<EntryPoint>]
let main argv =
    let numberedGrid =  File.ReadAllLines(argv.[0]) |> Array.indexed
    Simulate1 numberedGrid |> printfn "Part 1: %A"
    Simulate2 numberedGrid |> printfn "Part 2: %A"
    0
