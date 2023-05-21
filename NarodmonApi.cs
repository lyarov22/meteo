using Newtonsoft.Json;
using System;
using System.Data.Entity.Core.Metadata.Edm;
using System.Drawing;
using System.Drawing.Text;
using System.Net;
using System.Net.Http;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace meteo
{
    public class NarodmonApi
    {
        public async Task<string> GetData()
        {
            var api_key = "vXU31e7cGz1M7";
            var md5_app_id = "783f2f44a073ee00102b316f119b8ac4";
            var url = "https://narodmon.ru/api";

            var data = new
            {
                cmd = "sensorsValues",
                uuid = md5_app_id,
                api_key = api_key,
                sensors = new int[] { 86324, 97412, 60068, 97415, 97413 },
                trend = 1,
                lang = "ru"
            };

            var client = new HttpClient();

            var json = JsonConvert.SerializeObject(data);
            var content = new StringContent(json, System.Text.Encoding.UTF8, "application/json");

            try
            {
                var response = await client.PostAsync(url, content);

                if (response.IsSuccessStatusCode)
                {
                    var responseContent = await response.Content.ReadAsStringAsync();

                    return responseContent;


                }
                else
                {
                    MessageBox.Show($"Error: {response.StatusCode}"); // отображаем ошибку в текстовом поле
                }

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "\nCheck your internet connection");
                return null;

            }

            return null;

        }
    }
}
