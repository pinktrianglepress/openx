
/**
 *  @file
 *  Attach var and cookie handle functions.
 */

(function ($) {
  Drupal.behaviors.openx = {
    attach: function (context) {
      var blkstr=[];
      $.each(Drupal.settings.openx,function(idx2,val2) {
        if(typeof(val2) == 'string') {
          var str = idx2 + '=' + val2;
          blkstr.push(str);
        }
      });

      $.each(Drupal.settings.openx.cookie,function(idx2,val2) {
        var str = idx2 + '=' + $.cookie(val2);
        if($.cookie(val2)) {
          blkstr.push(str);
        }
      });

      var script = document.createElement('script');
      script.setAttribute('src', Drupal.settings.openx.openx_url.url + '?' + blkstr.join('&'));
      script.setAttribute('type', 'text/javascript');
      document.getElementsByTagName('head')[0].appendChild(script);

      $('div.openx_lazyload_ad:not(.openx-class-processed)',context).each(function () {
        $(this).addClass('openx-class-processed');
        $(this).lazyLoadAd({
          forceLoad : true, // Ad is loaded even if not visible. Default is false.
          timeout : 50,     // Timeout ad load
          syncAds : true,
          debug : false,    // For debug use : draw colors border depends on load status
        });
      });
    }
  };
})(jQuery);

