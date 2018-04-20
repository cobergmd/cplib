using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CJO.CpLib
{
    public class ValueManager
    {
        private Dictionary<string, string> _Defines = new Dictionary<string, string>();
        private Dictionary<string, Pattern> _Patterns = new Dictionary<string, Pattern>();

        public void Add(string name, string value)
        {
            if (_Defines.ContainsKey(name))
            {
                throw new ParseException("Duplicate token name found: " + name);
            }
            _Defines.Add(name, value);
        }

        public string Get(string name)
        {
            return _Defines[name];
        }

        public void AddPattern(Pattern pattern)
        {
            if (_Patterns.ContainsKey(pattern.Name))
            {
                throw new ParseException("Duplicate pattern found: " + pattern.Name);
            }
            _Patterns.Add(pattern.Name, pattern);
        }

        public Pattern GetPattern(string name)
        {
            return _Patterns[name];
        }

        public override string  ToString()
        {
            StringBuilder sb = new StringBuilder();
            sb.AppendLine("=== Defines ===");
            foreach(string name in _Defines.Keys)
            {
                sb.AppendLine("Define: " +
                    name + " Value: " +
                    _Defines[name]);
            }
            sb.AppendLine("=== Patterns ===");
            foreach (string name in _Patterns.Keys)
            {
                sb.AppendLine("Pattern: " + name);
                Pattern p = _Patterns[name];
                foreach (Map map in p.Maps)
                {
                    sb.Append("  In: " + map.In);
                    sb.AppendLine(" Out: " + map.Out);
                }
            }
            return sb.ToString();
        }
    }

    public class Pattern
    {
        public List<Map> Maps = new List<Map>();

        public string Name {get; set;}
    }

    public class PatternHolder
    {
        public List<Pattern> Patterns = new List<Pattern>();
    }

    public class Map
    {
        public string In { get; set; }
        public string Out { get; set; }
    }

    public class MapStack
    {
        public Stack<string> stack = new Stack<string>();

        public void Push(string s)
        {
            stack.Push(s);
        }

        public string Pop()
        {
            return stack.Pop();
        }

        public string Peek()
        {
            return stack.Peek();
        }
    }
}
