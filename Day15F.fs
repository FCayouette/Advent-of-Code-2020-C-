open System.IO

let rec GetNthNumberSpoken (m:Map<_,_>) last count target =
    if count = target then last
    else let nextCount = count + 1 
         let newMap = Map.add last count m
         match m.TryFind(last) with | None -> GetNthNumberSpoken newMap 0 nextCount target
                                    | Some x -> GetNthNumberSpoken newMap (count-x) nextCount target

[<EntryPoint>]
let main argv =     
    let input = File.ReadAllText(argv.[0]).Split(',') |> Array.map System.Int32.Parse 
    let last = input.[input.Length - 1]
    let order = Array.fold(fun map x -> Map.add x (map.Count+1) map) Map.empty input.[..input.Length-1]
    printfn "Part 1: %A" (GetNthNumberSpoken order last order.Count 2020)
    printfn "Part 2: %A" (GetNthNumberSpoken order last order.Count 30000000)
    0
