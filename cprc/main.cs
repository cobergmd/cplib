/* cplib - Coding and Parsing Library
 * Copyright © 2009 Craig Oberg.  All rights reserved.
 *
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CJO.CpLib
{
    class main
    {
        static void Main(string[] args)
        {
            AppDomain.CurrentDomain.UnhandledException += new UnhandledExceptionEventHandler(CurrentDomain_UnhandledException);
            main.Run(args);
        }

        static void CurrentDomain_UnhandledException(object sender, UnhandledExceptionEventArgs ue)
        {
            Object o = ue.ExceptionObject;
            Exception e = o as Exception;
            if (e != null)
            { // Report System.Exception info
                Console.WriteLine("Exception = " + e.GetType());
                Console.WriteLine("Message = " + e.Message);
                Console.WriteLine("FullText = " + e.ToString());
            }
            else
            { // Report exception Object info
                Console.WriteLine("Exception = " + o.GetType());
                Console.WriteLine("FullText = " + o.ToString());
            }
            Environment.Exit(1); // Shutting down
        }

        public static void Run(string[] args)
        {
            Console.WriteLine("Parse Rules Compiler");
            Console.WriteLine();

            if (args.Length < 2)
            {
                Console.WriteLine("Usage: cprc <inputfile> <outputfile>");
                return;
            }

            using (TextReader r = File.OpenText(args[0]))
            {
                RuleCompiler compiler = new RuleCompiler(r);
                compiler.Compile(args[1]);
            }
        }
    }
}
