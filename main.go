package main

import "github.com/webview/webview"

func main() {
	w := webview.New(false)
	defer w.Destroy()
	w.SetTitle("Weather Map")
	w.SetSize(1000, 800, webview.HintNone)
	w.Move(0, 36)
	//	w.SetHtml("Thanks for using webview!")
	w.Navigate("https://openweathermap.org/city/1859642")
	w.Run()
}
