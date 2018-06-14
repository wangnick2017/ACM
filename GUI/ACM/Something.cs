using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Security.Cryptography;
using Clr;

namespace ACM
{
    static class Something
    {
        public static void Merge(List<Ticket> list)
        {
            if (list == null || list.Count == 0)
                return;
            for (int i = 0; i < list.Count; ++i)
            {
                for (int j = i + 1; j < list.Count; ++j)
                {
                    Ticket t1 = list[i], t2 = list[j];
                    if (t1.trainID == t2.trainID && t1.dateFrom == t2.dateFrom && t1.locFrom == t2.locFrom && t1.locTo == t2.locTo)
                    {
                        for (int k = 0; k < t1.cnt; ++k)
                        {
                            t1.num[k] += t2.num[k];
                        }
                        list.RemoveAt(j);
                        --j;
                    }
                }
            }
        }

        public static bool EmailString(string email)
        {
            for (int i = 0, j = email.Length; i < j; ++i)
            {
                if (!Char.IsLower(email[i]) && !Char.IsUpper(email[i]) && !Char.IsNumber(email[i]) && email[i] != '@' && email[i] != '.')
                    return false;
            }
            return true;
        }

        public static string MD5Encrypt(string password)
        {
            MD5CryptoServiceProvider md5Hasher = new MD5CryptoServiceProvider();
            byte[] hashedDataBytes;
            hashedDataBytes = md5Hasher.ComputeHash(Encoding.GetEncoding("gb2312").GetBytes(password));
            StringBuilder tmp = new StringBuilder();
            foreach (byte i in hashedDataBytes)
            {
                tmp.Append(i.ToString("x2"));
            }
            return tmp.ToString();
        }
    }
}
