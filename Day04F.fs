open System.IO

let Consolidate (consolidated, (acc:string)) (raw:string) = 
    let filtered = String.filter(fun c -> c <> '\r') raw
    if filtered.Length = 0 then 
        (Array.append consolidated [| acc |], "")
    else match acc.Length with | 0 -> (consolidated, filtered)
                               | _ -> (consolidated, acc + " " + filtered)

let eyeColors = [|"amb"; "blu"; "brn"; "gry"; "grn"; "hzl"; "oth"|]

let ValidateHeight (x:string) =
    if (x.Length < 3) then false
    else
        let unit = x.[x.Length-2..]
        let value = System.Int32.Parse x.[..x.Length-3]
        if (unit = "cm" && value >= 150 && value <= 193) ||  
           (unit = "in" && value >= 59 && value <= 76) then true 
        else false

let ValidateFieldsPart2 (fields:string[])  =
    let hcl = fields.[3]
    let pid = fields.[6]
    if (pid.Length = 9 && hcl.Length = 7 && hcl.[0] ='#') then 
        match (Array.filter(fun x -> x = fields.[1]) eyeColors).Length with | 0 -> false | _-> ValidateHeight fields.[4]
    else false

let ValidateFields (fields: string[]) =
    let byr = System.Int32.Parse fields.[0]
    let eyr = System.Int32.Parse fields.[2]
    let iyr = System.Int32.Parse fields.[5]
    if byr < 1920 || byr > 2002 || eyr < 2020 || eyr > 2030 || iyr < 2010 || iyr > 2020 then false
    else ValidateFieldsPart2 fields
        

[<EntryPoint>]
let main argv =
    let data = Array.fold(Consolidate) ([||], "") <| File.ReadAllText(argv.[0]).Split('\n')
    let rawPassports = Array.append (fst data) [| snd data |] |> Array.map(fun x -> x.Split(' ') |> Array.filter(fun x -> x.[0] <> 'c') |> Array.sort)
    let part1 = Array.filter(fun (x:string[]) -> x.Length = 7) rawPassports
    printfn "Part 1: %A" part1.Length
    let part2 = Array.map(fun x -> Array.map(fun (s:string) -> s.[4..]) x) part1 |> Array.filter(ValidateFields)
    printfn "Part 2: %A" part2.Length
    0 // return an integer exit code
