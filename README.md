# Simple arithmetic library
## Test file format description
test [TESTNAME]
[caseTest|randTest]
caseTest ::= case: [A,B,...,N], params are decimal numbers
randTest ::= rand(N): params...
params ::= [unsigned(size),signed(size),range(a,b),]
