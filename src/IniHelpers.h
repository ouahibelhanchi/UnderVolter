// IniHelpers.h — Shared INI streaming-scan helpers used by NvramSetup.c
//                and SecureBootEnroll.c.  Both modules walk the loaded INI
//                buffer line-by-line tracking the current section state,
//                rather than doing keyed lookups (Config.c style).  These
//                helpers consolidate the previously duplicated primitives
//                into a single implementation.
//
// Conventions:
//   - All input strings are CHAR8 (ASCII).  Output paths are CHAR16 (UEFI).
//   - Match functions are case-insensitive (ASCII -32 uppercasing trick).
//   - Read functions advance past optional spaces / tabs before '=' and value.
//   - Section match tolerates whitespace inside brackets: "[ Name ]" matches.
#pragma once

#include <Uefi.h>

// Case-insensitive prefix match: returns TRUE if s starts with lit (ASCII).
// Does not advance s; caller uses IniStrLen8(lit) to skip past matched part.
BOOLEAN IniMatchCI(CONST CHAR8* s, CONST CHAR8* lit);

// strlen for CHAR8 (libc not available in UEFI pre-boot).
UINTN IniStrLen8(CONST CHAR8* s);

// Parse value after '=' as 0/1 boolean.  Whitespace tolerant around '=' and value.
// Returns Default if no '=' is found or the value is not exactly '0' or '1'.
BOOLEAN IniReadBool(CONST CHAR8* p, BOOLEAN Default);

// Parse value after '=' as unsigned decimal integer.  Whitespace tolerant.
// Stops at the first non-digit.  Returns Default if no '=' or no digits.
UINT32 IniReadUint(CONST CHAR8* p, UINT32 Default);

// Parse value after '=' as a filesystem path.  Converts '/' → '\' to normalise
// for UEFI's CHAR16 path APIs.  Strips trailing spaces.  Writes a zero-terminated
// CHAR16 string of at most OutLen-1 characters to Out.  Silent no-op if no '='.
VOID IniReadPath(CONST CHAR8* p, CHAR16* Out, UINTN OutLen);

// Whitespace-tolerant section header match.  p points to the character AFTER
// the opening '[' of a section line.  Returns TRUE when the bracketed name
// matches SectionName (case-insensitive), ignoring spaces / tabs around the
// name and before ']'.  Examples that all match section "SetupVar":
//     [SetupVar]   [setupvar]   [ SetupVar ]   [SetupVar  ]
// Does NOT advance p — caller is responsible for line skip after the test.
BOOLEAN IniSectionMatch(CONST CHAR8* p, CONST CHAR8* SectionName);
