open System.IO

let toIntArray (x:string) = x.Split(',') |> Array.map System.Int32.Parse

let parseCat (x:string) =
    let colon = x.IndexOf(':')
    let name = x.Substring(0, colon)
    name, Array.map System.Int32.Parse (x.Substring(colon + 2).Replace(" or ", "-").Split('-')) 

let ParseData (data:string[]) =
    let firstEmpty = Array.findIndex(fun x -> x="") data
    let cat = data.[0..firstEmpty-1] |> Array.map parseCat
    let ticket = toIntArray data.[firstEmpty + 2]
    cat, ticket, Array.map toIntArray data.[firstEmpty + 5..data.Length-1] 

let IsInvalid x (_,vals:'a[]) = 
    x < vals.[0] || x > vals.[3] || (x > vals.[1] && x < vals.[2])

let NonValid x = Array.fold(fun truth cat -> truth && IsInvalid x cat) true

let TicketInvalidValues cats = Array.fold(fun t v -> if NonValid v cats then t+v else t) 0 

let IsTicketInvalid cats = Array.fold(fun t v -> t || NonValid v cats) false 

let GetPos cats x = 
    Array.fold(fun (s:Set<_>) (c, cat) -> if IsInvalid x cat then s.Add(c) else s) Set.empty (Array.indexed cats)

let CheckPos ((possibilities:Set<_>[]), cats) ticket =
    Array.fold(fun p x -> Array.append(p) [|Set.difference possibilities.[p.Length] (GetPos cats x)|]) [||] ticket, cats

let IsDeparture (s:string) = s.Length > 9 && s.Substring(0,9) = "departure"

[<EntryPoint>]
let main argv =
    let cats, ticket, nearby = ParseData(File.ReadAllLines(argv.[0]))
    printfn "Part1 %A" <| Array.fold(fun total t -> total + TicketInvalidValues cats t) 0 nearby
    let validTickets = Array.filter(fun x -> not (IsTicketInvalid cats x)) nearby
    let possibilities, _ = Array.fold CheckPos ((Array.fold(fun t _ -> Array.append([|Set.ofSeq [0 .. cats.Length-1]|]) t) [||] cats), cats) validTickets
    let sorted = Array.indexed possibilities |> Array.sortBy(fun (_,x)->x.Count)
    let part2, _ = Array.fold(fun (prod, prev) (i, s) -> if (IsDeparture (fst cats.[Set.minElement(Set.difference s prev)])) then ((int64)ticket.[i] * prod, s)
                                                         else (prod, s)) (1L, Set.empty) sorted
    printfn "Part2 %A" part2
    0
