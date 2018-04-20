options {
	STATIC = false;
	OPTIMIZE_TOKEN_MANAGER = true;
	DEBUG_PARSER = false;
	IGNORE_CASE=true;
}

PARSER_BEGIN(RuleCompiler)

namespace CJO.CpLib;

using System;
using System.IO;
using System.Text;
using System.Collections;

class RuleCompiler {
    private char[] trimchars = new char[] {'\'','\"'};
	private ValueManager _Values = new ValueManager();
    private MapStack _InStack = new MapStack();
    private MapStack _OutStack = new MapStack();
    private StreamWriter _Outfile;

	public void Compile(string outfile)
	{

       _Outfile = File.CreateText(outfile);

		Statement();

        _Outfile.Close();

		System.Console.WriteLine(_Values.ToString());

		System.Console.WriteLine("done!");
	}

    private void PrintPattern(Pattern[] patterns, int curridx)
    {
        if (patterns.Length > curridx + 1)
            PrintPattern(patterns, curridx + 1);

        Pattern p = patterns[curridx];
        foreach (Map map in p.Maps)
        {
            _InStack.Push(map.In);
            _OutStack.Push(map.Out);
        }
    }
}

PARSER_END(RuleCompiler)

SKIP:
{
    " "
|   "\t"
|   "\r"
|   "\n"
|  <"//" (~["\n","\r"])* ("\n" | "\r" | "\r\n")>
|  <"/*" (~["*"])* "*" ("*" | ~["*","/"] (~["*"])* "*")* "/">
}

TOKEN :
{
	<DECIMAL_LITERAL: ["1"-"9"] (["0"-"9"])*>
|	<STRING_LITERAL: "\"" ( ~["\"","\\","\n","\r"] | "\\" ( ["n","t","b","r","f","\\","\'","\""] | ["0"-"7"] (["0"-"7"])?  | ["0"-"3"] ["0"-"7"] ["0"-"7"] | ( ["\n","\r"] | "\r\n")))* "\"">
}

TOKEN : /* Keywords and symbols */
{
	<DEFINE:"define">
|	<WEIGHT:"weight">
|	<PATTERN:"pattern">
|	<RULE:"rule">
|	<MAPPER:"=>">
|	<ASTERISK:"*">
}

TOKEN :
{
	<IDENTIFIER: <LETTER> (<LETTER> | <DIGIT>)*>
|	<#LETTER: ["$","A"-"Z","_","a"-"z"]>
|	<#DIGIT: ["0"-"9"]>
}

void Statement() : {}
{
	( TokenDefinition()  |
	  WeightDefinition() |
	  PatternDefinition() |
	  RuleDefinition() )*
}

void TokenDefinition() :
{
	Token tname = null;
	Token tvalue = null;
}
{
	( <DEFINE>
	  tname = <IDENTIFIER>
	  tvalue = <STRING_LITERAL>
	    { _Values.Add(tname.image, tvalue.image.Trim(trimchars)); }
	)
}

void WeightDefinition() :
{
	Token tname = null;
	Token tvalue = null;
}
{
	( <WEIGHT>
	  tname = <IDENTIFIER>
	  tvalue = <DECIMAL_LITERAL>
	    { _Values.Add(tname.image, tvalue.image); }
	)
}

void PatternDefinition() :
{
	Token t = null;
	Pattern pattern = new Pattern();
}
{
	( <PATTERN>
	t = <IDENTIFIER>
        {  pattern.Name = t.image; }
	"{" MapList(pattern) "}"
	    {  _Values.AddPattern(pattern); }
    )
}

void MapList(Pattern pattern) : {}
{
	Map(pattern) ( Map(pattern) )*
}

void Map(Pattern pattern) :
{
	string inmap = null;
	string outmap =  null;
}
{
	inmap = IdentList() <MAPPER>  outmap = IdentList() ";"
	{  Map m = new Map();
       m.In = inmap;
       m.Out = outmap;
       pattern.Maps.Add(m);
    }
}

string IdentList() :
{
	Token t = null;
	string val = null;
	StringBuilder sb = new StringBuilder();
}
{
	t = <IDENTIFIER>
        {   val = _Values.Get(t.image);
	        if (val == null) throw new ParseException("Define not found: " + t.image);
		    sb.Append(val);
	    }
	(LOOKAHEAD(2) "," t = <IDENTIFIER>
	    {   val = _Values.Get(t.image);
	        if (val == null) throw new ParseException("Define not found: " + t.image);
		    sb.Append(val);
	    }
	)*
	    { return sb.ToString(); }
}

void RuleDefinition() :
{
    Token t = null;
    string weight = null;
    Pattern[] patterns = null;
    StringBuilder mapstr = new StringBuilder();
}
{
	(
    <RULE>
    t = <IDENTIFIER>
    "["
    patterns = PatternList()
        {  if (patterns != null)
            {
                PrintPattern(patterns, 0);

                weight = _Values.Get(t.image);
     	        if (weight == null) throw new ParseException("Weight not found: " + t.image);

                mapstr.Append(weight + ",");
                foreach (string s in _InStack.stack)
                {
                    mapstr.Append(s + " ");
                }
                _InStack.stack.Clear();
                mapstr.Remove(mapstr.Length - 1, 1);
                mapstr.Append(",");
                foreach (string s in _OutStack.stack)
                {
                    mapstr.Append(s + " ");
                }
                _OutStack.stack.Clear();
                mapstr.Remove(mapstr.Length - 1, 1);
                _Outfile.WriteLine(mapstr.ToString());
            }
        }
    "]"
    )
}

Pattern[] PatternList() :
{
    Map[] maps;
    PatternHolder ph = new PatternHolder();
}
{
	maps = MapSelection()
    {  if (maps != null)
       {
           Pattern p = new Pattern();
           foreach (Map m in maps)
               p.Maps.Add(m);
           ph.Patterns.Add(p);
       }
    }
    (LOOKAHEAD(2) ","
    maps = MapSelection()
    {  if (maps != null)
       {
           Pattern p = new Pattern();
           foreach (Map m in maps)
               p.Maps.Add(m);
           ph.Patterns.Add(p);
       }
    }
    )*  { return ph.Patterns.ToArray(); }
}

Map[] MapSelection() :
{
    Token t = null;
    Token ti = null;
    int idx = -1;
    Map[] results;
    Pattern p = null;
}
{
    t = <IDENTIFIER> ":"
    (
    <ASTERISK>
         {  p = _Values.GetPattern(t.image);
	        if (p == null) throw new ParseException("Pattern not found: " + t.image);
            results = p.Maps.ToArray();
         }
    |
    ti = <DECIMAL_LITERAL>
         {  p = _Values.GetPattern(t.image);
	        if (p == null) throw new ParseException("Pattern not found: " + t.image);
            idx = Convert.ToInt32(ti.image) - 1;
            results = new Map[1];
            results[0] = p.Maps[idx];
         }
     )
         {  return results; }
}


