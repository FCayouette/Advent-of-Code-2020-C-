open System.IO

let FormatData (t, (mask:string), mem) (s:string) =
    if s.[1] = 'a' then
        if mask.Length > 0 then
            (Array.append t [|(mask, mem)|], s.[7..], [])
        else (t, s.[7..], [])
    else
        let v = s.Replace("mem[", "").Replace("] = ", " ").Split(' ') |> Array.map System.UInt64.Parse
        (t, mask, mem @ [(v.[0], v.[1])])

let rec GetModifiedValue (mask:string) power i value =
    if i > mask.Length then value
    else let newPower = power*2UL
         let nextI = i + 1
         match mask.[mask.Length - i] with | '1' -> GetModifiedValue mask newPower nextI value ||| power
                                           | '0' -> GetModifiedValue mask newPower nextI value &&& ~~~power
                                           | _ -> GetModifiedValue mask newPower nextI value

let Part1 m (mask:string, mem) =
    mem |> List.fold(fun a (address, value) -> Map.add address (GetModifiedValue mask 1UL 1 value) a) m

let rec WriteToModifiedAddress m address (mask:string) value power i =
    if i > mask.Length then Map.add address value m
    else let newPower = power*2UL
         let nextI = i + 1
         match mask.[mask.Length - i] with | '1' -> WriteToModifiedAddress m (address ||| power) mask value newPower nextI
                                           | '0' -> WriteToModifiedAddress m address mask value newPower nextI
                                           | _ -> let tmp = WriteToModifiedAddress m (address ||| power) mask value newPower nextI
                                                  WriteToModifiedAddress tmp (address &&& ~~~ power) mask value newPower nextI

let Part2 (m:Map<_,_>) (mask:string, mem) =
    mem |> List.fold(fun a (address, value) -> WriteToModifiedAddress a address mask value 1UL 1) m

let AddValuesInMemory m = Map.fold(fun total _ v -> total + v) 0UL m

[<EntryPoint>]
let main argv =
    let (tmpData, lastMask, lastMem) = File.ReadAllLines(argv.[0]) |> Array.fold(FormatData) ([||], "", [])
    let data = Array.append tmpData [|(lastMask, lastMem)|]
    Array.fold(Part1) Map.empty data |> AddValuesInMemory |> printfn "Part 1: %A"
    Array.fold(Part2) Map.empty data |> AddValuesInMemory |> printfn "Part 2: %A"
    0