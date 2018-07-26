/***
 * Copyright 2004-present Facebook. All Rights Reserved.
 */

open GenFlowCommon;

let createModulesMap = modulesMapFile => {
  let s = readFile(modulesMapFile);
  Str.split(Str.regexp("\n"), s)
  |> List.fold_left(
       (map, nextPairStr) =>
         if (nextPairStr != "") {
           let fromTo =
             Str.split(Str.regexp("="), nextPairStr) |> Array.of_list;
           assert(Array.length(fromTo) === 2);
           let moduleName: ModuleName.t = fromTo[0] |> ModuleName.fromString;
           let v: string = fromTo[1];
           ModuleNameMap.add(moduleName, v, map);
         } else {
           map;
         },
       ModuleNameMap.empty,
     );
};

let fileHeader = "/* @flow strict */\n";

let signFile = s => s;

let shimsFile = {
  let default =
    Sys.file_exists(Paths.defaultShimsFile()) ?
      Some(Paths.defaultShimsFile()) : None;
  ref(default);
};

let cli = () => {
  let setProjectRoot = Paths.setProjectRoot;
  let setShimsFile = s => shimsFile := Some(s |> Paths.absoluteFromProject);
  let getModulesMap = () =>
    switch (shimsFile^) {
    | None => Paths.defaultShimsFile() |> createModulesMap
    | Some(fileName) => fileName |> createModulesMap
    };
  let setCmtAdd = s => {
    let splitColon = Str.split(Str.regexp(":"), s) |> Array.of_list;
    assert(Array.length(splitColon) === 2);
    let cmt: string = splitColon[0];
    let mlast: string = splitColon[1];
    print_endline("  Add " ++ cmt ++ "  " ++ mlast);
    cmt
    |> GenFlowMain.processCmtFile(
         ~fileHeader,
         ~signFile,
         ~modulesMap=getModulesMap(),
       );
    exit(0);
  };
  let setCmtRm = s => {
    let splitColon = Str.split(Str.regexp(":"), s) |> Array.of_list;
    assert(Array.length(splitColon) === 1);
    let cmtAbsolutePath: string = splitColon[0];
    /* somehow the CMT hook is passing an absolute path here */
    let cmt = cmtAbsolutePath |> Paths.relativePathFromBsLib;
    let outputFile = cmt |> Paths.getOutputFile;
    print_endline("  Remove " ++ cmt);
    if (Sys.file_exists(outputFile)) {
      Unix.unlink(outputFile);
    };
    exit(0);
  };
  let speclist = [
    (
      "--setProjectRoot",
      Arg.String(setProjectRoot),
      "set the root of the bucklescript project",
    ),
    (
      "--shims",
      Arg.String(setShimsFile),
      "Specify a file containing a list of module overrides, one per line."
      ++ " Example(-shims shims.txt) where each line is of the form 'Module=OtherModule'. "
      ++ "E.g. 'ReasonReact=ReactShim' finds and imports 'ReactShim.shim.js'.",
    ),
    ("-cmt-add", Arg.String(setCmtAdd), "compile a .cmt[i] file"),
    ("-cmt-rm", Arg.String(setCmtRm), "remove a .cmt[i] file"),
  ];
  let usage = "genFlow";
  Arg.parse(speclist, print_endline, usage);
};

cli();