package main

import "github.com/webview/webview"

func main() {
	w := webview.New(false)
	defer w.Destroy()
	w.SetTitle("Weather Map")
	w.SetSize(1200, 800, webview.HintNone)
//	w.Move(200,200)
//	w.SetHtml("Thanks for using webview!")
	w.Navigate("https://openweathermap.org/city/1850916")
	w.Run()
}
