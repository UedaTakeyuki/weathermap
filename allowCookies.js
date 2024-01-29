//location.href = "https://openweathermap.org/city/1852278"
/*
const delaySec = 10 * 1000 // 10 seconds
console.log("KEROYOOOOON!!!!!")
setTimeout(startallowCookie, 1000)
function startallowCookie(){
  console.log("KERO!!!!!YOOOOON!!!!!")
  ptimer = setInterval(allowCookie,1000/*delaySec*//*)
}
*/
function allowCookie(){
  console.log("KERO!!!!!YOOOO!!!!ON!!!!!")
  const footer_panel = document.querySelector('.stick-footer-panel__link');
  if (footer_panel != null) {
    footer_panel.click();
    console.log("footer_panel.clicked")
//    clearInterval(ptimer);
  } else {
    console.log("calling next allowCookies")
  }
}
allowCookie()
