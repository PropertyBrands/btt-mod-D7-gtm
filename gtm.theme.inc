<?php

/**
 * Theme callback from GTM container
 */
function theme_gtm_container(&$variables) {
  // Early exit on no track.
  if(!$variables['track']) {
    return false;
  }

  // Initial step.
  $init = '<script type="text/javascript">var '. $variables['data_layer'] .' = [];</script>';
  $push = '';
  foreach(module_invoke_all('gtm_data_layer') as $data_layer_push) {
    $layer = array(
      '#theme' => 'gtm_datalayer',
      '#dataLayer' => $data_layer_push,
    );
    $push .= drupal_render($layer);
  }

  // Generate scripts for each id generated.
  $script = '';

  // Process GTM track additions.
  foreach ([$variables['id'], $variables['id_extra']] as $id) {
    $script .= _gtm_add_gtm($id, $variables);
  }

  // Combine all.
  $script = <<<EOS
<!-- Google Tag Manager -->
$init
$push
$script
<!-- End Google Tag Manager -->
EOS;

  return $script;
}

/**
 * Process the addition on the GTM library.
 */
function _gtm_add_gtm($id, $variables) {
  // Empty ids, nothing processed.
  if (empty($id)) {
    return '';
  }

  // Build a suffix to use in js variables generated.
  $suffix = '_' . md5($id) . '_' . rand();

  // Build script tags.
  $noscript = <<<EOS
<noscript><iframe src="//www.googletagmanager.com/ns.html?id={$id}"
 height="0" width="0" style="display:none;visibility:hidden" loading="lazy"></iframe></noscript>
EOS;

  // Get GTM loading vars.
  $gtm_load_delay = $variables['gtm_load_delay'];
  $gtm_load_on_user_interaction = $variables['gtm_load_on_user_interaction'];

  $script = <<<EOS
<script type="text/javascript">
  // Track user interaction.
  let gtmUserInteracted$suffix = !{$gtm_load_on_user_interaction};

  // Keep looping until js objects are available.
  let gtmCheckAllInPlace$suffix = setInterval(function() {
    if (typeof jQuery === 'undefined') {
      return;
    }

    // Done with the checking.
    clearInterval(gtmCheckAllInPlace$suffix);

    // Self calling function.
    (function(w,d,s,l,i,$,Drupal) {
      // Detect user interactions.
      $('body').once(function() {
        if (gtmUserInteracted$suffix) {
          return;
        }

        $('body').mousemove(function() {
          gtmUserInteracted$suffix = true;
        });

        $('body').bind('touchmove', function() {
          gtmUserInteracted$suffix = true;
        });

        $('body').keydown(function(e) {
          if ((e.keyCode == 9) || (e.keyCode == 13)) {
            gtmUserInteracted$suffix = true;
          }
        });
      });

      // Run when page is ready, by using jQuery.
      $(function() {
        // Wait on user interaction.
        let gtmCheckingUserInteraction = setInterval(function() {
          // No interaction, keep waiting.
          if (!gtmUserInteracted$suffix) {
            return;
          }

          // Interaction detected.
          clearInterval(gtmCheckingUserInteraction);

          // Give it a time before running the code.
          setTimeout(function() {
            w[l]=w[l]||[];
            w[l].push({'gtm.start':new Date().getTime(),event:'gtm.js'});
            var f=d.getElementsByTagName(s)[0];
            var j=d.createElement(s);
            var dl=l!='dataLayer'?'&l='+l:'';
            j.src='//www.googletagmanager.com/gtm.js?id='+i+dl;
            j.type='text/javascript';
            j.async=true;
            f.parentNode.insertBefore(j,f);
          }, {$gtm_load_delay});
        }, 250);
      });
    })(window,document,'script','{$variables['data_layer']}','{$id}',jQuery,Drupal);
  }, 500);
</script>
EOS;

  // Custom script that only enabled tag manager if cookie tracking is allowed
  // @see https://github.com/bluetent/mod-btm_cookie_compliance#disableenable-setting-cookies
  $script_cookie_compliance = <<<EOS
<script type="text/javascript">
  // Track user interaction.
  let gtmUserInteracted$suffix = !{$gtm_load_on_user_interaction};

  // Keep looping until js objects are available.
  let gtmCheckAllInPlace$suffix = setInterval(function() {
    if (typeof jQuery === 'undefined') {
      return;
    }

    // Done with the checking.
    clearInterval(gtmCheckAllInPlace$suffix);

    // Self calling function.
    (function(w,d,s,l,i,$,Drupal) {
      // Detect user interactions.
      $('body').once(function() {
        if (gtmUserInteracted$suffix) {
          return;
        }

        $('body').mousemove(function() {
          gtmUserInteracted$suffix = true;
        });

        $('body').bind('touchmove', function() {
          gtmUserInteracted$suffix = true;
        });

        $('body').keydown(function(e) {
          if ((e.keyCode == 9) || (e.keyCode == 13)) {
            gtmUserInteracted$suffix = true;
          }
        });
      });

      // Run when page is ready, by using jQuery.
      $(function() {
        // Wait on user interaction.
        let gtmCheckingUserInteraction = setInterval(function() {
          // No interaction, keep waiting.
          if (!gtmUserInteracted$suffix) {
            return;
          }

          // Interaction detected.
          clearInterval(gtmCheckingUserInteraction);

          // Give it a time before running the code.
          setTimeout(function() {
            if (allowCookies()) {
              w[l]=w[l]||[];
              w[l].push({'gtm.start':new Date().getTime(),event:'gtm.js'});
              var f=d.getElementsByTagName(s)[0];
              var j=d.createElement(s);
              var dl=l!='dataLayer'?'&l='+l:'';
              j.src='//www.googletagmanager.com/gtm.js?id='+i+dl;
              j.type='text/javascript';
              j.async=true;
              f.parentNode.insertBefore(j,f);
            } else {
              $(d).on('cookieset', function (e, allowCookies, mode) {
                // Determine if cookie can be set or not
                if (allowCookies) {
                  w[l]=w[l]||[];
                  w[l].push({'gtm.start':new Date().getTime(),event:'gtm.js'});
                  var f=d.getElementsByTagName(s)[0];
                  var j=d.createElement(s);
                  var dl=l!='dataLayer'?'&l='+l:'';
                  j.src='//www.googletagmanager.com/gtm.js?id='+i+dl;
                  j.type='text/javascript';
                  j.async=true;
                  f.parentNode.insertBefore(j,f);
                }
              });
            }
          }, {$gtm_load_delay});
        }, 250);
      });
    })(window,document,'script','{$variables['data_layer']}','{$id}',jQuery,Drupal);
  }, 500);
</script>
EOS;

  if ($variables['btm_cookie_compliance_enable']) {
    $script = $script_cookie_compliance;
  }

  return $noscript . $script;
}

/**
 * Implements hook_preprocess_HOOK()
 */
function gtm_preprocess_gtm_container(&$variables) {
  if(gtm_should_track()) {
    $variables['track'] = TRUE;
    $variables['id'] = variable_get('gtm');
    $variables['id_extra'] = variable_get('gtm_extra');
    $variables['data_layer'] = variable_get('datalayer', 'dataLayer');
    $variables['btm_cookie_compliance_enable'] = variable_get('btm_cookie_compliance_enable', 0);
    $variables['gtm_load_delay'] = variable_get('gtm_load_delay', 0);
    $variables['gtm_load_on_user_interaction'] = variable_get('gtm_load_on_user_interaction', 0);
  }
}

/**
 * Theme callback for dataLayer
 */
function theme_gtm_datalayer(&$variables) {
  if(gtm_should_track()) {
    $build = array(
      '#theme' => 'html_tag',
      '#tag' => 'script',
      '#attributes' => array(
        'type' => 'text/javascript'
      ),
      '#value' => $variables['dataLayer'],
    );
    return drupal_render($build);
  }
  return FALSE;
}
