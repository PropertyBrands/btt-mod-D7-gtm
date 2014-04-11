<?php

/**
 * Theme callback from GTM container
 */
function theme_gtm_container(&$variables) {
  if($variables['track'] == TRUE) {
    $layers = '';
    foreach(module_invoke_all('gtm_data_layer') as $dl) {
      $layer = array(
        '#prefix' => '<script type="text/javascript">',
        '#markup' => $dl,
        '#suffix' => '</script>',
      );
      $layers .= drupal_render($layer);
    }
    return $layers.'
    <!-- Google Tag Manager -->
<noscript><iframe src="//www.googletagmanager.com/ns.html?'. $variables['id'] .'"
height="0" width="0" style="display:none;visibility:hidden"></iframe></noscript>
<script>(function(w,d,s,l,i){w[l]=w[l]||[];w[l].push({\'gtm.start\':
new Date().getTime(),event:\'gtm.js\'});var f=d.getElementsByTagName(s)[0],
j=d.createElement(s),dl=l!=\'dataLayer\'?\'&l=\'+l:\'\';j.async=true;j.src=
\'//www.googletagmanager.com/gtm.js?id=\'+i+dl;f.parentNode.insertBefore(j,f);
})(window,document,\'script\',\''. $variables['data_layer'] .'\',\''. $variables['id'] .'\');</script>
<!-- End Google Tag Manager -->';
  }
  return FALSE;
}

/**
 * Implements hook_preprocess_HOOK()
 */
function gtm_preprocess_gtm_container(&$variables) {
  if(gtm_should_track()) {
    $variables['track'] = TRUE;
    $variables['id'] = variable_get('gtm');
    $variables['data_layer'] = variable_get('datalayer', 'dataLayer');
  }
}

/**
 * Theme callback for dataLayer
 */
function theme_gtm_datalayer(&$variables) {
  if(gtm_should_track()) {
    return $variables['dataLayer'];
  }
  return FALSE;
}