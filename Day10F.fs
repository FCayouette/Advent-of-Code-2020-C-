open System.IO

let Part2 (acc, count) (l,h) =
    if (h-l) < 3 then (acc, count+1)
    else match count-1 with 
                          | 1 -> (acc*2L, 0)
                          | 2 -> (acc*4L, 0)
                          | 3 -> (acc*7L, 0)
                          | _ -> (acc, 0)

[<EntryPoint>]
let main argv =
    let partial = Array.append [|0|] (File.ReadAllLines(argv.[0]) |> Array.map System.Int32.Parse |> Array.sort)
    let adaptors = Array.append partial [|3 + Array.last partial|]
    let pairedAdaptors = Array.pairwise adaptors
    let part1 = Array.fold(fun (ones, threes) (f,s) -> match s-f with |1 ->(ones+1, threes) | 3 -> (ones, threes+1) |_->(ones,threes)) (0,0) pairedAdaptors
    
    printfn "Part 1: %A" (fst part1 * snd part1)
    printfn "Part 2: %A" (fst (Array.fold(Part2) (1L, 0) pairedAdaptors))
    0 // return an integer exit code
