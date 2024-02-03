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
  const optin_button = document.querySelector('.gdpr-banner__button--opt-in');
  if (optin_button != null) {
    optin_button.click();
    console.log("optin_button clicked")
//    clearInterval(ptimer);
  } else {
    console.log("calling next allowCookies")
  }
}
allowCookie()
