<?php
/**
 * @file
 * Drupal Module: Google Tag Manager
 * Provides info about hooks available.
 */

/**
 * Alters the track status of a page.
 */
function hook_gtm_should_track_alter(&$should_track, $context) {
  // Change $should_track based on passed $context.
}
