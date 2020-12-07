open System.IO

let Consolidate (consolidated, (acc:string), count) (raw:string) =
    let filtered = String.filter(fun c -> c <> '\r') raw
    if filtered.Length = 0 then
        (Array.append consolidated [|(acc.ToCharArray(), count)|], "", 0)
    else (consolidated, acc + filtered, count + 1)

[<EntryPoint>]
let main argv =
    let (t1, t2, t3) = Array.fold(Consolidate) ([||], "", 0) <| File.ReadAllText(argv.[0]).Split('\n')
    let groups = Array.append(t1) [| (t2.ToCharArray(), t3) |]
    printfn "Part 1: %A" (Array.fold(fun total (x,_) -> (x |> Array.distinct).Length + total) 0 groups)
    printfn "Part 2: %A" (Array.fold(fun total (x, count) -> (x |> Array.countBy(id) |> Array.filter(fun (_,x) -> x=count)).Length + total) 0 groups)
    0 // return an integer exit code
