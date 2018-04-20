using System;
using System.Text;
using System.Runtime.InteropServices;

namespace CJO.CpLib
{
    /// <summary>
    /// .NET Interop wrapper of cplib address parser.
    /// </summary>

    [Serializable]
    public enum CpAddressType : int
    {
        UnknownAddress,
        Residential,
        Business,
        PoBox,
        Military,
        RuralRoute,
        HighwayContract,
        GeneralDelivery
    }

    [StructLayout(LayoutKind.Sequential), Serializable]
    public struct CpAddress
    {
        public CpAddressType AddressType;
        public string AddressNumber;
        public string PreDirectional;
        public string StreetName;
        public string Suffix;
        public string PostDirectional;
        public string SecondaryIdentifier;
        public string SecondaryRange;
        public string MailstopCode;
        public string Department;
        public string BoxNumber;
        public string MilitaryUnitType;
        public string MilitaryUnitNumber;
        public string MilitaryBoxNumber;
        public string MilitaryShipName;
        public string RuralRouteNumber;
        public string RuralRouteBoxNumber;
        public string HighwayContractNumber;
        public string City;
        public string State;
        public string Zip5;
        public string Zip4;
    }

    public class CpParser : MarshalByRefObject, IDisposable
    {
        [DllImport("cplib.dll", EntryPoint = "cp_parser_init",
        SetLastError = true, CharSet = CharSet.Auto, ExactSpelling = true,
        CallingConvention = CallingConvention.StdCall)]
        private static extern void CpParserInit();

        [DllImport("cplib.dll", EntryPoint = "cp_parser_parse",
        SetLastError = true, CharSet = CharSet.Auto, ExactSpelling = true,
        CallingConvention = CallingConvention.StdCall)]
        private static extern void CpParserParse([MarshalAs(UnmanagedType.LPStr)] string line,
                                                 ref CpAddress address);

        [DllImport("cplib.dll", EntryPoint = "cp_parser_destroy",
        SetLastError = true, CharSet = CharSet.Auto, ExactSpelling = true,
        CallingConvention = CallingConvention.StdCall)]
        private static extern void CpParserDestroy();

        public CpParser()
        {
            CpParserInit();
        }

        public CpAddress Parse(string line)
        {
            CpAddress addr = CreateAddressStruct();
            CpParserParse(line, ref addr);
            return addr;
        }

        #region IDisposable Members

        public void Dispose()
        {
            CpParserDestroy();
        }

        #endregion

        private CpAddress CreateAddressStruct()
        {
            CpAddress addr = new CpAddress();
            addr.AddressType = CpAddressType.UnknownAddress;
            addr.AddressNumber = "";
            addr.PreDirectional = "";
            addr.StreetName = "";
            addr.Suffix = "";
            addr.PostDirectional = "";
            addr.SecondaryIdentifier = "";
            addr.SecondaryRange = "";
            addr.MailstopCode = "";
            addr.Department = "";
            addr.BoxNumber = "";
            addr.MilitaryUnitType = "";
            addr.MilitaryUnitNumber = "";
            addr.MilitaryBoxNumber = "";
            addr.MilitaryShipName = "";
            addr.RuralRouteNumber = "";
            addr.RuralRouteBoxNumber = "";
            addr.HighwayContractNumber = "";
            addr.City = "";
            addr.State = "";
            addr.Zip5 = "";
            addr.Zip4 = "";

            return addr;
        }
    }
}