open System.IO

let rec ExtractBags (x:string) bags =
  let space = x.IndexOf(' ')-1
  let num = System.Int32.Parse(x.[..space])
  let nameEnd = x.IndexOfAny [|','; '.'|]
  let fullName = x.[space+2..nameEnd-1]
  let name = fullName.[..fullName.IndexOf("bag")-2]
  let newBags = Array.append bags [|(num, name)|]
  match x.[nameEnd] = ',' with | false -> newBags
                               | true -> ExtractBags x.[nameEnd+2..] newBags
  
let ExtractRules (x:string) =
    let a = x.IndexOf(" bags contain ")-1
    let name = x.[..a]
    let contains = x.[a+15..]
    if contains = "no other bags." then (name,  [||])
    else (name, ExtractBags contains [||])
    
let rec CanContainShinyGold (x:string) contains (rules:Map<_,_>) =
    if x = "shiny gold" then true
    else contains |> Array.exists(fun (_,col) -> CanContainShinyGold col rules.[col] rules)

let rec CountBag (x:string) (rules:Map<_,_>) =
     Array.fold(fun total (n, color)-> total + n * CountBag color rules) 1 rules.[x]

[<EntryPoint>]
let main argv =
    let rules = File.ReadAllLines(argv.[0]) |> Array.map(ExtractRules) |> Map.ofArray
    (rules |> Map.filter(fun x contains -> CanContainShinyGold x contains rules) |> Map.count) - 1 |> printfn "Part 1: %A"
    (CountBag "shiny gold" rules)-1 |> printfn "Part 2: %A"
    0 // return an integer exit code
